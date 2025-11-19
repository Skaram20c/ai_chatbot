#include "OpenAIFallback.h"
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include "json.hpp"

using json = nlohmann::json;

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

// Utility to escape double quotes
std::string OpenAIFallback::escapeJson(const std::string &input)
{
    std::string out = input;
    std::replace(out.begin(), out.end(), '"', '\'');
    return out;
}

std::string OpenAIFallback::askOpenAI(const std::string &question)
{
    const char *key = std::getenv("OPENAI_API_KEY");
    if (!key)
        return "OpenAI API key missing.";

    std::string apiKey = key;
    std::string prompt = escapeJson(question);

    // Curl command
    std::string cmd =
        "curl -s -X POST https://api.openai.com/v1/chat/completions "
        "-H \"Content-Type: application/json\" "
        "-H \"Authorization: Bearer " + apiKey + "\" "
                   "-d \"{"
                   "\\\"model\\\":\\\"gpt-4o-mini\\\","
                   "\\\"messages\\\":[{\\\"role\\\":\\\"user\\\",\\\"content\\\":\\\"" + prompt + "\\\"}]"
                   "}\"";

    std::string result;
    char buffer[512];

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return "OpenAI request failed.";

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    pclose(pipe);

    // Parse JSON safely
    try {
        json j = json::parse(result);
        return j["choices"][0]["message"]["content"].get<std::string>();
    }
    catch (...) {
        return "OpenAI returned an unexpected format.";
    }
}
