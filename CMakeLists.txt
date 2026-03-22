cmake_minimum_required(VERSION 3.22)
project(ComplexityAnalyzer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Qt6 auto‑tools ─────────────────────────────────────────────────────────
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ── Find Qt6 ────────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Network)

# ── Source files ────────────────────────────────────────────────────────────
set(ENGINE_SOURCES
    src/TokenProcessor.cpp
    src/ASTBuilder.cpp
    src/Analyzer.cpp
)

set(GUI_SOURCES
    gui/SyntaxHighlighter.cpp
    gui/ChartHelper.cpp
    gui/MainWindow.cpp
)

add_executable(ComplexityAnalyzer
    main.cpp
    ${ENGINE_SOURCES}
    ${GUI_SOURCES}
)

target_include_directories(ComplexityAnalyzer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/gui
)

target_link_libraries(ComplexityAnalyzer PRIVATE
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
)

# ── Deploy Qt DLLs next to the exe (MinGW) ──────────────────────────────────
if(WIN32)
    add_custom_command(TARGET ComplexityAnalyzer POST_BUILD
        COMMAND "${Qt6_DIR}/../../../bin/windeployqt6.exe"
                --no-translations
                $<TARGET_FILE:ComplexityAnalyzer>
        COMMENT "Running windeployqt6..."
    )
endif()
