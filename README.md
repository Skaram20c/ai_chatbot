# 📘 **AI Academic Companion**  
*A modern intelligent academic chatbot built with Qt, Machine Learning, n8n automation, and OpenAI fallback.*

![Qt](https://img.shields.io/badge/Qt-6.5-brightgreen)
![C++](https://img.shields.io/badge/C++-17-blue)
![Python](https://img.shields.io/badge/Python-3.10-yellow)
![n8n](https://img.shields.io/badge/n8n-Automation-orange)
![OpenAI](https://img.shields.io/badge/OpenAI-Fallback-purple)
![License: OPEN](https://img.shields.io/badge/License-MIT-lightgrey)

---

## 🧭 **Table of Contents**
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Tech Stack](#tech-stack)
- [API Endpoints](#api-endpoints)
- [Future Improvements](#future-improvements)
- [Contributors/Developers](#contributors/Developers)

---

# 📖 **Overview**

The **AI Academic Companion** is a desktop-based intelligent chatbot designed for university students to quickly access academic information such as admissions, tuition, OSAP, programs, exam policies, deadlines, and more.  
It provides:

- A **polished Qt GUI**  
- **ML-powered question classification**  
- **OpenAI fallback via n8n**  
- **Email-based chat transcript export**  
- **Professional PDF transcript generation**

This project demonstrates full-stack integration between C++/Qt, Python ML, REST APIs, automation workflows, and cloud services.

---

# 🚀 **Features**

### 🖥️ **Qt Desktop Application**
- Clean professional UI with:
  - Sidebar (New Chat + Chat History)
  - Rounded chat bubbles
  - Modern blue/white theme
  - Avatar icons for AI and user
  - Timestamp formatting
- PDF export with branded layout

### 🤖 **Machine Learning Backend**
- Python (Flask API)
- TF-IDF vectorizer
- Logistic Regression / SVM classifier
- Trained on 500+ university FAQ samples

### 🔄 **n8n Automation**
- OpenAI fallback when ML cannot classify
- Gmail transcript sender
- PDF attachment support
- Webhook integration between Qt and ML

### 🌐 **Ngrok Integration**
- Secure tunneling between Qt → ML API

---

# 🧩 **Architecture**

                  ┌────────────────────────┐
                  │     Qt Frontend        │
                  │   (C++ / QWidgets)     │
                  └─────────────┬──────────┘
                                │ JSON Query
                                ▼
                 ┌────────────────────────────┐
                 │   ML Backend (Flask API)   │
                 │  - TF-IDF Vectorizer       │
                 │  - Classifier (SVM/LR)     │
                 └─────────────┬──────────────┘
                               │ fallback?: true
                               ▼
                 ┌────────────────────────────┐
                 │        n8n Workflow        │
                 │ - OpenAI GPT fallback      │
                 │ - Email + PDF exporter     │
                 └─────────────┬──────────────┘
                               │ JSON response
                               ▼
                  ┌────────────────────────┐
                  │      Qt Frontend       │
                  │   Renders final output │
                  └────────────────────────┘
                  


---

# 🛠️ **Tech Stack**

### **Frontend**
- C++17  
- Qt 6 (Widgets, QPainter, QPdfWriter)

### **Backend**
- Python 3.10  
- Flask API  
- scikit-learn  
- pandas, numpy, joblib  

### **Automation**
- n8n  
- OpenAI GPT API  
- Gmail API Integration

### **Tools**
- Google Colab  
- Ngrok  
- GitHub


---

# 📡 API Endpoints

### **POST /predict**

This endpoint receives a user query from the Qt Application and returns either:

- A direct ML-based response  
- Or a fallback response generated through n8n + OpenAI

**Request**
```json
{
  "query": "How do I apply for OSAP?"
}
```

**Response (ML Success)**
```json
{
  "fallback": false,
  "response": "You can apply for OSAP at ontario.ca/osap..."
}
```

**Response Fallback**
```json
{
  "fallback": true,
  "response": "Data not available!"
}
```

# ⚠️ Known Issues
- Ngrok free-tier tunnels expire frequently and break API connection.
- Long messages may overflow the bubble layout.
- n8n workflow must remain active for fallback to function.
- Dataset quality directly affects ML classification accuracy.

# 🛠️ Future Improvements

- Deploy ML backend to cloud (Railway / Render / AWS / GCP).
- Add voice input using speech-to-text.
- Add text-to-speech for AI response speaking.
- Add user accounts with cloud-based sync.
- Implement dark mode + customizable themes.
- Expand dataset to 2000–5000 FAQs.
- Replace ngrok with a domain + SSL.
- Add chatbot analytics dashboard for admin.

# 👥 Contributors

Karam Singh    - Lead I    
Pooja Bera     - Lead II   
Deepan Bhatta  - Lead III 
