#ifndef MLFAQSYSTEM_H
#define MLFAQSYSTEM_H

#include <string>
#include <vector>

class FAQ
{
public:
    FAQ() {}
    FAQ(const std::string &q, const std::string &a)
        : question(q), answer(a) {}

    std::string question;
    std::string answer;
};

class MLFAQSystem
{
public:
    MLFAQSystem();

    // Core functions
    void loadFromFile(const std::string &filename);
    std::string ask(const std::string &userInput);

    // Utilities
    std::string callMlApi(const std::string &question);
    std::string toLower(const std::string &s);
    std::vector<std::string> splitWords(const std::string &s);
    std::string fallbackCSV(const std::string &userInput);
    std::string callOpenAI(const std::string &question);

    // Set API URL
    void setApiUrl(const std::string &url);

private:
    std::string apiURL;
    std::vector<FAQ> data;
};

#endif
