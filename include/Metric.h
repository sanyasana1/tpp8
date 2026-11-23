#pragma once

#include <string>

struct Metric
{
    int id = 0;
    int serverId = 0;
    std::string metricName;
    double metricValue = 0.0;
    std::string unit;
    std::string recordedAt;
};
