#pragma once
#include "../src/ComplexityEstimator.h"
#include <QtCharts/QChart>

class ChartHelper {
public:
    // Returns a fully configured QChart* — caller takes ownership
    static QChart* buildChart(ComplexityClass highlighted);
};
