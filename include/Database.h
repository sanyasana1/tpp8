#pragma once

#include <stdexcept>
#include <string>
#include <sqlite3.h>
#include <sqlite_orm/sqlite_orm.h>
#include "Server.h"
#include "Metric.h"

inline auto createStorage(const std::string& filename)
{
    using namespace sqlite_orm;
    return make_storage(
        filename,
        make_table(
            "Server",
            make_column("id", &Server::id, primary_key().autoincrement()),
            make_column("name", &Server::name, not_null()),
            make_column("ipAddress", &Server::ipAddress, not_null()),
            make_column("environment", &Server::environment, not_null())
        ),
        make_table(
            "Metric",
            make_column("id", &Metric::id, primary_key().autoincrement()),
            make_column("serverId", &Metric::serverId),
            make_column("metricName", &Metric::metricName, not_null()),
            make_column("metricValue", &Metric::metricValue, not_null()),
            make_column("unit", &Metric::unit, not_null()),
            make_column("recordedAt", &Metric::recordedAt, not_null()),
            foreign_key(&Metric::serverId).references(&Server::id)
        )
    );
}

using DbStorage = decltype(createStorage(""));

inline DbStorage makeConfiguredStorage(const std::string& filename)
{
    DbStorage storage = createStorage(filename);

    storage.on_open = [](sqlite3* db)
    {
        if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;",
                         nullptr, nullptr, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error("Cannot enable foreign key constraints");
        }
    };

    storage.open_forever();
    storage.sync_schema();
    return storage;
}
