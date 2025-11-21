#include "MLFAQSystem.h"
#include "OpenAIFallback.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include "json.hpp"

using json = nlohmann::json;


#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

MLFAQSystem::MLFAQSystem()
{
    apiURL = "";
}

void MLFAQSystem::setApiUrl(const std::string &url)
{
    apiURL = url;
}

std::string MLFAQSystem::toLower(const std::string &s)
{
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return r;
}

std::vector<std::string> MLFAQSystem::splitWords(const std::string &s)
{
    std::vector<std::string> words;
    std::istringstream iss(s);
    std::string w;
    while (iss >> w) words.push_back(w);
    return words;
}

void MLFAQSystem::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    getline(file, line);

    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string q, cat, label, a;

        getline(ss, q, ',');
        getline(ss, cat, ',');
        getline(ss, label, ',');
        getline(ss, a);

        if (!q.empty() && !a.empty())
        {
            data.emplace_back(toLower(q), toLower(a));
        }
    }
}

std::string MLFAQSystem::fallbackCSV(const std::string &userInput)
{
    auto words = splitWords(toLower(userInput));

    int bestScore = -1;
    std::string bestAns = "Sorry, I don’t have an answer for that.";

    for (auto &faq : data)
    {
        auto qWords = splitWords(faq.question);
        int score = 0;

        for (auto &w : words)
            if (std::find(qWords.begin(), qWords.end(), w) != qWords.end())
                score++;

        if (score > bestScore)
        {
            bestScore = score;
            bestAns = faq.answer;
        }
    }
    return bestAns;
}

std::string MLFAQSystem::callMlApi(const std::string &userInput)
{
    if (apiURL.empty())
        return "__ML_ERROR__";

    std::string endpoint = apiURL;

    std::string cmd =
        "curl -s -X POST -H \"Content-Type: application/json\" "
        "-d \"{\\\"query\\\":\\\"" + userInput + "\\\"}\" "
                      "\"" + endpoint + "\"";

    std::string result;
    char buffer[512];

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return "__ML_ERROR__";

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    pclose(pipe);

    if (result.empty())
        return "__ML_ERROR__";

    // Trim newline at the end
    if (!result.empty() && result.back() == '\n')
        result.pop_back();

    //return result;
    try {
        json j = json::parse(result);
        return result;  // return full JSON to ask()
    }
    catch (...) {
        return "__ML_ERROR__";
    }
}

std::string MLFAQSystem::ask(const std::string &userInput)
{
    // 1. Call ML (n8n or Colab)
    std::string ml_raw = callMlApi(userInput);

    if (ml_raw == "__ML_ERROR__" || ml_raw.empty()) {
        std::string aiAns = callOpenAI(userInput);
        return aiAns.empty() ? fallbackCSV(userInput) : aiAns;
    }

    json j;
    try {
        j = json::parse(ml_raw);
    }
    catch (...) {
        std::string aiAns = callOpenAI(userInput);
        return aiAns.empty() ? fallbackCSV(userInput) : aiAns;
    }

    // 2. Handle fallback properly
    bool isFallback = false;

    if (j.contains("fallback"))
    {
        if (j["fallback"].is_boolean())
            isFallback = j["fallback"].get<bool>();

        else if (j["fallback"].is_string())
        {
            std::string f = j["fallback"].get<std::string>();
            std::transform(f.begin(), f.end(), f.begin(), ::tolower);
            isFallback = (f == "true");
        }
    }

    if (isFallback)
    {
        std::string aiAns = callOpenAI(userInput);
        return aiAns.empty() ? fallbackCSV(userInput) : aiAns;
    }

    // fallback = false, check for response
    if (j.contains("response") && j["response"].is_string()) {
        return j["response"].get<std::string>();
    }

    // Alternative naming (e.g., n8n returns "answer")
    if (j.contains("answer") && j["answer"].is_string()) {
        return j["answer"].get<std::string>();
    }

    std::string aiAns = callOpenAI(userInput);
    return aiAns.empty() ? fallbackCSV(userInput) : aiAns;
}

std::string MLFAQSystem::callOpenAI(const std::string &question)
{
    return OpenAIFallback::askOpenAI(question);
}

