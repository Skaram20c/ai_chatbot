# 📘 **AI Academic Companion**  
*A modern intelligent academic chatbot built with Qt, Machine Learning, n8n automation, and OpenAI fallback.*

![Qt](https://img.shields.io/badge/Qt-6.5-brightgreen)
![C++](https://img.shields.io/badge/C++-17-blue)
![Python](https://img.shields.io/badge/Python-3.10-yellow)
![n8n](https://img.shields.io/badge/n8n-Automation-orange)
![OpenAI](https://img.shields.io/badge/OpenAI-Fallback-purple)
![License: MIT](https://img.shields.io/badge/License-MIT-lightgrey)

---

## 🧭 **Table of Contents**
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Setup Instructions](#setup-instructions)
  - [ML Backend (Google Colab)](#ml-backend-google-colab)
  - [Qt Frontend](#qt-frontend)
  - [n8n Workflow Setup](#n8n-workflow-setup)
- [API Endpoints](#api-endpoints)
- [Screenshots](#screenshots)
- [PDF Export Example](#pdf-export-example)
- [Known Issues](#known-issues)
- [Future Improvements](#future-improvements)
- [Contributors](#contributors)
- [License](#license)

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
                  

