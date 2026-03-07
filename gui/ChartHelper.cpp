#include "ChartHelper.h"
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>
#include <QColor>
#include <QPen>
#include <QFont>
#include <cmath>

static void addSeries(QChart* chart,
                      const QString& name,
                      const QColor& color,
                      int penWidth,
                      const std::vector<std::pair<double,double>>& pts)
{
    auto* series = new QLineSeries(chart);
    series->setName(name);
    QPen pen(color);
    pen.setWidth(penWidth);
    series->setPen(pen);
    for (auto& p : pts) {
        series->append(p.first, p.second);
    }
    chart->addSeries(series);
}

QChart* ChartHelper::buildChart(ComplexityClass highlighted) {
    auto* chart = new QChart();
    chart->setTitle("Complexity Growth Curves");
    chart->setBackgroundBrush(QColor("#1e1e2e"));
    chart->setTitleBrush(QColor("#cdd6f4"));
    QFont tf; tf.setPointSize(10); tf.setBold(true);
    chart->setTitleFont(tf);
    chart->legend()->setLabelColor(QColor("#cdd6f4"));
    chart->legend()->setBackgroundVisible(false);
    chart->setAnimationOptions(QChart::NoAnimation);

    struct CurveData {
        ComplexityClass cls;
        QString         name;
        QColor          base;
    };
    static const CurveData curves[] = {
        {ComplexityClass::O1,      "O(1)",        "#a6e3a1"},
        {ComplexityClass::OLogN,   "O(log n)",    "#89dceb"},
        {ComplexityClass::ON,      "O(n)",        "#89b4fa"},
        {ComplexityClass::ONLogN,  "O(n log n)",  "#cba6f7"},
        {ComplexityClass::ON2,     "O(n²)",       "#fab387"},
        {ComplexityClass::ON2LogN, "O(n² log n)", "#f38ba8"},
        {ComplexityClass::ON3,     "O(n³)",       "#f5c2e7"},
    };

    for (const auto& c : curves) {
        bool hl = (c.cls == highlighted) ||
                  (highlighted == ComplexityClass::ONK && c.cls == ComplexityClass::ON3);

        QColor col(c.base);
        if (!hl) col.setAlphaF(0.25f);
        int w = hl ? 4 : 2;

        std::vector<std::pair<double,double>> pts;
        for (int x = 1; x <= 20; ++x) {
            double y = 0;
            switch (c.cls) {
            case ComplexityClass::O1:      y = 1;                              break;
            case ComplexityClass::OLogN:   y = std::log2(x);                  break;
            case ComplexityClass::ON:      y = x;                              break;
            case ComplexityClass::ONLogN:  y = x * std::log2(x);              break;
            case ComplexityClass::ON2:     y = (double)x * x;                 break;
            case ComplexityClass::ON2LogN: y = (double)x * x * std::log2(x); break;
            case ComplexityClass::ON3:     y = (double)x * x * x;             break;
            default: y = x; break;
            }
            if (y <= 500) pts.push_back({x, y});
        }

        addSeries(chart, c.name, col, w, pts);
    }

    // Set up axes manually (avoid createDefaultAxes qgraphics issues)
    auto* axX = new QValueAxis(chart);
    axX->setTitleText("n (input size)");
    axX->setTitleBrush(QColor("#cdd6f4"));
    axX->setLabelsBrush(QColor("#cdd6f4"));
    axX->setGridLineColor(QColor("#313244"));
    axX->setLinePen(QPen(QColor("#45475a")));
    axX->setRange(1, 20);

    auto* axY = new QValueAxis(chart);
    axY->setTitleText("Operations");
    axY->setTitleBrush(QColor("#cdd6f4"));
    axY->setLabelsBrush(QColor("#cdd6f4"));
    axY->setGridLineColor(QColor("#313244"));
    axY->setLinePen(QPen(QColor("#45475a")));
    axY->setRange(0, 500);

    chart->addAxis(axX, Qt::AlignBottom);
    chart->addAxis(axY, Qt::AlignLeft);

    // Attach all series to both axes
    for (auto* s : chart->series()) {
        s->attachAxis(axX);
        s->attachAxis(axY);
    }

    return chart;
}
