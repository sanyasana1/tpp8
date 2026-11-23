#include "SQLiteServerRepository.h"

SQLiteServerRepository::SQLiteServerRepository(const std::string& databaseFile)
    : storage_(makeConfiguredStorage(databaseFile))
{
}

int SQLiteServerRepository::add(const Server& server)
{
    return storage_.insert(server);
}

std::optional<Server> SQLiteServerRepository::findById(int id)
{
    return storage_.get_optional<Server>(id);
}

std::vector<Server> SQLiteServerRepository::findAll()
{
    return storage_.get_all<Server>();
}

bool SQLiteServerRepository::update(const Server& server)
{
    if (!storage_.get_optional<Server>(server.id))
    {
        return false;
    }
    storage_.update(server);
    return true;
}

bool SQLiteServerRepository::remove(int id)
{
    if (!storage_.get_optional<Server>(id))
    {
        return false;
    }
    storage_.remove<Server>(id);
    return true;
}
