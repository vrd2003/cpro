#pragma once

// ── Application stylesheet ────────────────────────────────────────────────────
// Extracted from MainWindow.cpp for maintainability.
// Consumed by MainWindow::applyTheme() via setStyleSheet(k_stylesheet).

inline constexpr const char* k_stylesheet = R"(
QMainWindow, QWidget {
    background-color: #1e1e2e;
    color: #cdd6f4;
    font-family: "Segoe UI", sans-serif;
    font-size: 13px;
}
QPlainTextEdit {
    background-color: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
    padding: 6px;
    selection-background-color: #585b70;
    font-family: "Consolas", "Courier New", monospace;
    font-size: 13px;
}
QPlainTextEdit:focus { border: 1px solid #89b4fa; }
QPushButton#analyzeBtn {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                 stop:0 #1e66f5, stop:1 #89b4fa);
    color: #1e1e2e;
    border: none;
    border-radius: 8px;
    padding: 10px 24px;
    font-weight: bold;
    font-size: 14px;
}
QPushButton#analyzeBtn:hover {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                 stop:0 #89b4fa, stop:1 #1e66f5);
}
QPushButton#analyzeBtn:pressed { background: #313244; }
QComboBox {
    background: #313244;
    color: #cdd6f4;
    border: 1px solid #45475a;
    border-radius: 6px;
    padding: 4px 8px;
}
QComboBox::drop-down { border: none; }
QComboBox QAbstractItemView {
    background: #313244;
    color: #cdd6f4;
    selection-background-color: #45475a;
}
QTreeWidget {
    background: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
    alternate-background-color: #1e1e2e;
}
QTreeWidget::item:hover      { background: #313244; }
QTreeWidget::item:selected   { background: #45475a; color: #cdd6f4; }
QTreeWidget QHeaderView::section {
    background: #313244;
    color: #89b4fa;
    border: none;
    padding: 4px;
    font-weight: bold;
}
QListWidget {
    background: #181825;
    color: #cdd6f4;
    border: 1px solid #313244;
    border-radius: 6px;
}
QListWidget::item { padding: 6px; border-bottom: 1px solid #313244; }
QListWidget::item:hover    { background: #313244; }
QListWidget::item:selected { background: #45475a; }
QGroupBox {
    border: 1px solid #313244;
    border-radius: 6px;
    margin-top: 24px;
    padding-top: 8px;
    color: #89b4fa;
    font-weight: bold;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 10px;
    top: 2px;
    background: transparent;
    padding: 0 4px;
}
QStatusBar { background: #181825; color: #a6adc8; }
QScrollBar:vertical, QScrollBar:horizontal {
    background: #181825;
    width: 8px; height: 8px;
    border: none;
}
QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
    background: #45475a; border-radius: 4px;
}
QScrollBar::add-line, QScrollBar::sub-line { height: 0; width: 0; }
QTabWidget::pane {
    border: 1px solid #313244;
    border-radius: 6px;
    background: #181825;
}
QTabBar::tab {
    background: #1e1e2e;
    color: #a6adc8;
    padding: 8px 16px;
    border: 1px solid transparent;
    border-bottom: 1px solid #313244;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
    font-weight: bold;
}
QTabBar::tab:selected {
    background: #181825;
    color: #89b4fa;
    border: 1px solid #313244;
    border-bottom: none;
}
QTabBar::tab:hover:!selected { background: #313244; }
QTableWidget {
    background: #181825;
    color: #cdd6f4;
    border: none;
    gridline-color: #313244;
    selection-background-color: #45475a;
}
QHeaderView::section {
    background: #313244;
    color: #89b4fa;
    padding: 6px;
    border: none;
    font-weight: bold;
}
QTableWidget::item { padding: 4px; }
)";
