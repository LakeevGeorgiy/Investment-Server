#pragma once

#include <memory>

class DatabaseConnectionInterface {
public:

    static std::unique_ptr<DatabaseConnectionInterface>& GetInstance(std::string_view connection_string);
    auto ExecQuery(std::string query, auto& params);

};