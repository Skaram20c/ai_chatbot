#ifndef MLFAQSYSTEM_H
#define MLFAQSYSTEM_H

#include <string>
#include <vector>

class FAQ
{
// Simple container for FAQ entries used for CSV fallback.
public:
    FAQ() {}
    FAQ(const std::string &q, const std::string &a)
        : question(q), answer(a) {}

    std::string question;
    std::string answer;
};

/**
 * @brief MLFAQSystem
 * Handles:
 *   1. Local CSV fallback matching
 *   2. ML API call (Colab / n8n)
 *   3. OpenAI fallback
 *
 * Provides a robust layered approach:
 *      ML → OpenAI → CSV fallback
 */
class MLFAQSystem
{
public:
    MLFAQSystem();

    void loadFromFile(const std::string &filename);             // Load fallback data from CSV
    std::string ask(const std::string &userInput);              // Main query handler
    void setApiUrl(const std::string &url);                     // Set API URL

private:
    // ---- Helpers ----
    std::string toLower(const std::string &s);
    std::vector<std::string> splitWords(const std::string &s);
    std::string fallbackCSV(const std::string &userInput);      // Fallback function
    std::string callMlApi(const std::string &question);         // Utility/Model call
    std::string callOpenAI(const std::string &question);        // Utility/Model call

private:
    std::string apiURL;
    std::vector<FAQ> data;
    bool isWebhookMode = true;
};

#endif
