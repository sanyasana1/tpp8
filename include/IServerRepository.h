#pragma once

#include <optional>
#include <vector>
#include "Server.h"

class IServerRepository
{
public:
    virtual ~IServerRepository() = default;

    virtual int add(const Server& server) = 0;

    virtual std::optional<Server> findById(int id) = 0;
    virtual std::vector<Server> findAll() = 0;

    virtual bool update(const Server& server) = 0;
    virtual bool remove(int id) = 0;
};
