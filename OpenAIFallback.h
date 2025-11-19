#ifndef OPENAIFALLBACK_H
#define OPENAIFALLBACK_H

#include <string>

class OpenAIFallback {
public:
    static std::string askOpenAI(const std::string &question);

private:
    static std::string escapeJson(const std::string &input);
};

#endif
