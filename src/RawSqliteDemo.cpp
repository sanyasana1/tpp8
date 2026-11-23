#include "Demos.h"

#include <filesystem>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <sqlite3.h>
#include "Server.h"

namespace {

const char* kCreateServerTable = R"(
CREATE TABLE IF NOT EXISTS Server (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    ipAddress TEXT NOT NULL,
    environment TEXT NOT NULL
);)";

[[noreturn]] void throwSqliteError(sqlite3* db)
{
    throw std::runtime_error(sqlite3_errmsg(db));
}

int insertServerRaw(sqlite3* db, const Server& server)
{
    const char* sql =
        "INSERT INTO Server (name, ipAddress, environment) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throwSqliteError(db);
    }

    int rc = sqlite3_bind_text(stmt, 1, server.name.c_str(), -1, SQLITE_TRANSIENT);
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_bind_text(stmt, 2, server.ipAddress.c_str(), -1, SQLITE_TRANSIENT);
    }
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_bind_text(stmt, 3, server.environment.c_str(), -1, SQLITE_TRANSIENT);
    }
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_step(stmt);
    }

    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throwSqliteError(db);
    }

    sqlite3_finalize(stmt);
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

std::optional<Server> findServerByIdRaw(sqlite3* db, int id)
{
    const char* sql =
        "SELECT id, name, ipAddress, environment FROM Server WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throwSqliteError(db);
    }

    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        throwSqliteError(db);
    }

    std::optional<Server> result;
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        Server server;
        server.id = sqlite3_column_int(stmt, 0);
        server.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        server.ipAddress = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        server.environment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        result = server;
    }
    else if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throwSqliteError(db);
    }

    sqlite3_finalize(stmt);
    return result;
}

}

void runRawSqliteDemo()
{
    const std::string databaseFile = "raw_demo.db";
    std::filesystem::remove(databaseFile);

    std::cout << "\n=== SQLITE C API DEMONSTRATION ===\n";

    sqlite3* db = nullptr;
    if (sqlite3_open(databaseFile.c_str(), &db) != SQLITE_OK)
    {
        std::string message = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error(message);
    }

    try
    {
        char* errorMessage = nullptr;
        if (sqlite3_exec(db, kCreateServerTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
        {
            std::string message = errorMessage ? errorMessage : "unknown error";
            sqlite3_free(errorMessage);
            throw std::runtime_error(message);
        }

        int id = insertServerRaw(db, Server{0, "raw-node-01", "10.0.3.1", "production"});
        std::cout << "Inserted server ID (C API): " << id << '\n';

        auto found = findServerByIdRaw(db, id);
        if (found)
        {
            std::cout << "Found: " << found->name << " | "
                      << found->ipAddress << " | " << found->environment << '\n';
        }
        else
        {
            std::cout << "Server not found\n";
        }
    }
    catch (...)
    {
        sqlite3_close(db);
        throw;
    }

    sqlite3_close(db);
}
