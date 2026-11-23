#include "Demos.h"

#include <iostream>
#include <tuple>
#include "Database.h"
#include "SQLiteServerRepository.h"

using namespace sqlite_orm;

namespace {

void printServer(const Server& server)
{
    std::cout << server.id << " | " << server.name << " | "
              << server.ipAddress << " | " << server.environment << '\n';
}

}

void runRepositoryDemo()
{
    std::cout << std::boolalpha;
    std::cout << "\n=== REPOSITORY (CRUD) DEMONSTRATION ===\n";

    SQLiteServerRepository sqliteRepository("server_monitor.db");
    IServerRepository& repository = sqliteRepository;

    Server server{0, "lab-node-01", "10.0.2.10", "development"};
    int id = repository.add(server);
    std::cout << "\nCREATE: inserted ID " << id << '\n';

    auto found = repository.findById(id);
    std::cout << "\nREAD:\n";
    if (found)
    {
        printServer(*found);
    }

    if (found)
    {
        found->environment = "staging";
        std::cout << "\nUPDATE: updated = " << repository.update(*found) << '\n';
        auto afterUpdate = repository.findById(id);
        if (afterUpdate)
        {
            std::cout << "New environment: " << afterUpdate->environment << '\n';
        }
    }

    auto allServers = repository.findAll();
    std::cout << "\nREAD ALL: " << allServers.size() << " servers\n";
    for (const auto& item : allServers)
    {
        printServer(item);
    }

    std::cout << "\nDELETE: removed = " << repository.remove(id) << '\n';
    std::cout << "Record exists after deletion: "
              << repository.findById(id).has_value() << '\n';
    std::cout << "Delete non-existing ID 999: "
              << repository.remove(999) << '\n';
}

void runQueryDemo()
{
    auto storage = makeConfiguredStorage("server_monitor.db");

    std::cout << "\n=== QUERY DEMONSTRATION: FILTER, SORT, LIMIT ===\n";

    std::cout << "\n1. CPU metrics above 50%:\n";
    auto highCpu = storage.get_all<Metric>(
        where(c(&Metric::metricName) == "cpu_usage"
              and c(&Metric::metricValue) > 50.0),
        order_by(&Metric::metricValue).desc()
    );
    for (const auto& metric : highCpu)
    {
        std::cout << "serverId " << metric.serverId << " | "
                  << metric.metricValue << " " << metric.unit << " | "
                  << metric.recordedAt << '\n';
    }

    std::cout << "\n2. Production servers sorted by name:\n";
    auto production = storage.get_all<Server>(
        where(c(&Server::environment) == "production"),
        order_by(&Server::name).asc()
    );
    for (const auto& server : production)
    {
        printServer(server);
    }

    std::cout << "\n3. Top 3 metric values:\n";
    auto top = storage.get_all<Metric>(
        order_by(&Metric::metricValue).desc(),
        limit(3)
    );
    for (const auto& metric : top)
    {
        std::cout << metric.serverId << " | " << metric.metricName
                  << " | " << metric.metricValue << '\n';
    }
}

void runAggregateDemo()
{
    auto storage = makeConfiguredStorage("server_monitor.db");

    std::cout << "\n=== AGGREGATE QUERY DEMONSTRATION ===\n";

    std::cout << "\n1. Metric count: " << storage.count<Metric>() << '\n';

    std::cout << "2. Sum of all metric values: "
              << storage.sum(&Metric::metricValue) << '\n';

    std::cout << "3. Average CPU usage: "
              << storage.avg(&Metric::metricValue,
                             where(c(&Metric::metricName) == "cpu_usage"))
              << " %\n";

    std::cout << "\n4. Average value by server (GROUP BY serverId):\n";
    auto rows = storage.select(
        columns(&Metric::serverId, avg(&Metric::metricValue)),
        group_by(&Metric::serverId)
    );
    for (const auto& [serverId, average] : rows)
    {
        std::cout << "Server ID: " << serverId
                  << " | Average: " << average << '\n';
    }
}

void runJoinDemo()
{
    auto storage = makeConfiguredStorage("server_monitor.db");

    std::cout << "\n=== JOIN QUERY DEMONSTRATION ===\n";
    std::cout << "\nMetrics above 80 with server information:\n";

    auto rows = storage.select(
        columns(
            &Server::name,
            &Server::environment,
            &Metric::metricName,
            &Metric::metricValue,
            &Metric::recordedAt
        ),
        inner_join<Metric>(on(c(&Server::id) == &Metric::serverId)),
        where(c(&Metric::metricValue) > 80.0),
        order_by(&Metric::metricValue).desc()
    );

    for (const auto& [name, environment, metricName, value, recordedAt] : rows)
    {
        std::cout << name << " | " << environment << " | " << metricName
                  << " | " << value << " | " << recordedAt << '\n';
    }
}
