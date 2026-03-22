#pragma once
#include "../src/ComplexityClass.h"
#include <QtCharts/QChart>

class ChartHelper {
public:
    // Returns a fully configured QChart* — caller takes ownership
    static QChart* buildChart(ComplexityClass highlighted);
};
