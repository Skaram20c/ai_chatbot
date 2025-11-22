#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

class AppConfig {
public:
    static QString getApiUrl() {
        return "http://localhost:5678/webhook/ai_chatbot";
        //return "https://clerkly-unpresumptive-yolando.ngrok-free.dev/predict";
    }
};

#endif // APPCONFIG_H
