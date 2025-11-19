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
    if (endpoint.back() == '/')
        endpoint += "predict";
    else
        endpoint += "/predict";

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

    try {
        json j = json::parse(result);
        return j.dump();  // return full JSON to ask()
    }
    catch (...) {
        return "__ML_ERROR__";
    }
}


std::string MLFAQSystem::ask(const std::string &userInput)
{
    std::string ml_raw = callMlApi(userInput);

    // If ML could not return anything → fallback
    if (ml_raw == "__ML_ERROR__" || ml_raw.empty())
        return callOpenAI(userInput);

    json j;
    try {
        j = json::parse(ml_raw);
    }
    catch (...) {
        return callOpenAI(userInput);
    }

    // If ML requests fallback (low confidence, no dataset match)
    if (j.contains("fallback") && j["fallback"].get<bool>() == true)
        return callOpenAI(userInput);

    // If ML has a proper response
    if (j.contains("response"))
        return j["response"].get<std::string>();

    // Unexpected format → fallback to OpenAI
    return callOpenAI(userInput);
}


std::string MLFAQSystem::callOpenAI(const std::string &question)
{
    return OpenAIFallback::askOpenAI(question);
}
