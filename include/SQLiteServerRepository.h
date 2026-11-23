#pragma once

#include <string>
#include "Database.h"
#include "IServerRepository.h"

class SQLiteServerRepository : public IServerRepository
{
public:
    explicit SQLiteServerRepository(const std::string& databaseFile);

    int add(const Server& server) override;
    std::optional<Server> findById(int id) override;
    std::vector<Server> findAll() override;
    bool update(const Server& server) override;
    bool remove(int id) override;

private:
    DbStorage storage_;
};
