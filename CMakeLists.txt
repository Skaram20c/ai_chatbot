cmake_minimum_required(VERSION 3.16)

project(ai_chatbot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets PrintSupport)

set(SOURCES
    main.cpp
    AppConfig.h
    MainWindow.h
    MainWindow.cpp
    ChatWindow.h
    ChatWindow.cpp
    GradientLabel.h
    GradientLabel.cpp
    MLFAQSystem.h
    MLFAQSystem.cpp
    ChatBubble.h
    ChatBubble.cpp
    SidebarWidget.h
    SidebarWidget.cpp
    Resources.qrc
)

qt_add_resources(RESOURCES styles/chatstyle.qss)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ai_chatbot
        MANUAL_FINALIZATION
        ${SOURCES}
        MainWindow.h
        SidebarWidget.h SidebarWidget.cpp


        Resources.qrc
        GradientLabel.h GradientLabel.cpp
        ChatBubble.h ChatBubble.cpp
        MLFAQSystem.h MLFAQSystem.cpp
        OpenAIFallback.h OpenAIFallback.cpp
        NotificationSender.h NotificationSender.cpp
        ChatPDFExporter.h ChatPDFExporter.cpp

    )
else()
    add_executable(ai_chatbot ${SOURCES})
endif()

target_link_libraries(ai_chatbot PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ai_chatbot)
endif()
