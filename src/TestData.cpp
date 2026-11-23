#include "TestData.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include "Database.h"

void prepareTestData()
{
    const std::string databaseFile = "server_monitor.db";
    std::filesystem::remove(databaseFile);

    auto storage = makeConfiguredStorage(databaseFile);

    std::vector<Server> servers = {
        {0, "web-01",    "10.0.0.11", "production"},
        {0, "db-01",     "10.0.0.21", "production"},
        {0, "backup-01", "10.0.0.31", "staging"},
        {0, "dev-01",    "10.1.0.5",  "development"},
        {0, "mail-01",   "10.0.0.41", "production"}
    };

    std::vector<int> serverIds;
    for (const auto& server : servers)
    {
        serverIds.push_back(storage.insert(server));
    }

    std::vector<Metric> metrics = {
        {0, serverIds[0], "cpu_usage",  35.5, "%", "2026-11-01 10:00"},
        {0, serverIds[0], "ram_usage",  62.0, "%", "2026-11-01 10:00"},
        {0, serverIds[0], "disk_usage", 48.2, "%", "2026-11-01 10:00"},

        {0, serverIds[1], "cpu_usage",  78.9, "%", "2026-11-01 10:05"},
        {0, serverIds[1], "ram_usage",  91.4, "%", "2026-11-01 10:05"},
        {0, serverIds[1], "disk_usage", 73.0, "%", "2026-11-01 10:05"},

        {0, serverIds[2], "cpu_usage",  12.3, "%", "2026-11-01 10:10"},
        {0, serverIds[2], "ram_usage",  40.1, "%", "2026-11-01 10:10"},
        {0, serverIds[2], "disk_usage", 88.7, "%", "2026-11-01 10:10"},

        {0, serverIds[3], "cpu_usage",  55.0, "%", "2026-11-01 10:15"},
        {0, serverIds[3], "ram_usage",  33.5, "%", "2026-11-01 10:15"},
        {0, serverIds[3], "disk_usage", 20.0, "%", "2026-11-01 10:15"},

        {0, serverIds[4], "cpu_usage",  67.8, "%", "2026-11-01 10:20"},
        {0, serverIds[4], "ram_usage",  71.2, "%", "2026-11-01 10:20"},
        {0, serverIds[4], "disk_usage", 59.4, "%", "2026-11-01 10:20"}
    };

    for (const auto& metric : metrics)
    {
        storage.insert(metric);
    }

    std::cout << "Test database prepared\n";
    std::cout << "Server records: " << storage.count<Server>() << '\n';
    std::cout << "Metric records: " << storage.count<Metric>() << '\n';
}
