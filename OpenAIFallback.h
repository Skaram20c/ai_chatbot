#ifndef OPENAIFALLBACK_H
#define OPENAIFALLBACK_H

#include <string>

/**
 * @brief Provides fallback communication with OpenAI API.
 *
 * Designed as a last-resort fallback when:
 *   - ML Model is offline
 *   - n8n fails
 *   - Dataset CSV cannot answer
 *
 * Uses curl and environment variable OPENAI_API_KEY.
 */
class OpenAIFallback {
public:
    /**
     * @brief Sends the user's question to OpenAI Chat Completion API.
     * @return AI response text, or a meaningful fallback error message.
     */
    static std::string askOpenAI(const std::string &question);

private:
    /**
     * @brief Escapes double quotes inside the JSON payload.
     *        Prevents malformed JSON when injecting user queries.
     */
    static std::string escapeJson(const std::string &input);
};

#endif
