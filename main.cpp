#include <QApplication>
#include <QPalette>
#include <QColor>
#include "gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("C++ DSA Code Analyzer");
    app.setOrganizationName("DSA Tools");

    // Force Fusion style for consistent look across platforms
    app.setStyle("Fusion");

    // Override palette with dark base colours (real theming via stylesheet)
    QPalette p;
    p.setColor(QPalette::Window,          QColor("#1e1e2e"));
    p.setColor(QPalette::WindowText,      QColor("#cdd6f4"));
    p.setColor(QPalette::Base,            QColor("#181825"));
    p.setColor(QPalette::AlternateBase,   QColor("#313244"));
    p.setColor(QPalette::Text,            QColor("#cdd6f4"));
    p.setColor(QPalette::Button,          QColor("#313244"));
    p.setColor(QPalette::ButtonText,      QColor("#cdd6f4"));
    p.setColor(QPalette::Highlight,       QColor("#89b4fa"));
    p.setColor(QPalette::HighlightedText, QColor("#1e1e2e"));
    p.setColor(QPalette::ToolTipBase,     QColor("#313244"));
    p.setColor(QPalette::ToolTipText,     QColor("#cdd6f4"));
    app.setPalette(p);

    MainWindow w;
    w.show();
    return app.exec();
}
