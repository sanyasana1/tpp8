#pragma once

#include <optional>
#include <vector>
#include "Database.h"

class MetricRepository
{
public:
    explicit MetricRepository(DbStorage& storage) : storage_(storage) {}

    int add(const Metric& metric)
    {
        return storage_.insert(metric);
    }

    std::optional<Metric> findById(int id)
    {
        return storage_.get_optional<Metric>(id);
    }

    std::vector<Metric> findByServerId(int serverId)
    {
        using namespace sqlite_orm;
        return storage_.get_all<Metric>(
            where(c(&Metric::serverId) == serverId),
            order_by(&Metric::recordedAt).asc()
        );
    }

private:
    DbStorage& storage_;
};
