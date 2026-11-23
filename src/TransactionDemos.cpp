#include "Demos.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include "Database.h"
#include "MetricRepository.h"

using namespace sqlite_orm;

namespace {

void printCounts(DbStorage& storage, const char* title)
{
    std::cout << title << '\n';
    std::cout << "Servers: " << storage.count<Server>() << '\n';
    std::cout << "Metrics: " << storage.count<Metric>() << '\n';
}

}

void runTransactionDemo()
{
    auto storage = makeConfiguredStorage("server_monitor.db");
    MetricRepository metrics(storage);

    std::cout << "\n=== TRANSACTION (COMMIT) ===\n";
    printCounts(storage, "\nBefore transaction:");

    bool committed = storage.transaction([&]()
    {
        int serverId = storage.insert(Server{0, "app-01", "10.0.0.51", "production"});
        metrics.add(Metric{0, serverId, "cpu_usage", 22.4, "%", "2026-11-01 11:00"});
        return true;
    });

    std::cout << "\nTransaction committed: " << std::boolalpha << committed << '\n';
    printCounts(storage, "\nAfter transaction:");
}

void runRollbackDemo()
{
    auto storage = makeConfiguredStorage("server_monitor.db");
    MetricRepository metrics(storage);

    std::cout << "\n=== ROLLBACK DEMONSTRATION ===\n";
    printCounts(storage, "\nBefore transaction:");

    try
    {
        storage.transaction([&]()
        {
            int serverId = storage.insert(Server{0, "cache-01", "10.0.0.61", "production"});
            std::cout << "Server inserted inside transaction, ID: " << serverId << '\n';

            metrics.add(Metric{0, -1, "cpu_usage", 10.0, "%", "2026-11-01 11:05"});
            return true;
        });
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nTransaction failed: " << ex.what() << '\n';
        std::cout << "Changes rolled back\n";
    }

    printCounts(storage, "\nAfter transaction:");

    auto cache = storage.get_all<Server>(where(c(&Server::name) == "cache-01"));
    std::cout << "cache-01 exists: " << std::boolalpha << !cache.empty() << '\n';
}

void runWithoutTransactionDemo()
{
    const std::string databaseFile = "server_no_transaction.db";
    std::filesystem::remove(databaseFile);

    auto storage = makeConfiguredStorage(databaseFile);
    MetricRepository metrics(storage);

    std::cout << "\n=== WITHOUT TRANSACTION ===\n";

    try
    {
        int serverId = storage.insert(Server{0, "cache-01", "10.0.0.61", "production"});
        std::cout << "Server inserted, ID: " << serverId << '\n';

        metrics.add(Metric{0, -1, "cpu_usage", 10.0, "%", "2026-11-01 11:05"});
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nOperation failed: " << ex.what() << '\n';
    }

    printCounts(storage, "\nAfter operations (no transaction):");
}
