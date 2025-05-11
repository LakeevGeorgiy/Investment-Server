#pragma once

#include <pqxx/pqxx>
#include <memory>

class DatabaseConnection {
private:

    pqxx::connection conn_;
    static std::unique_ptr<DatabaseConnection> db_;

    DatabaseConnection(std::string_view connection_string);

public:

    static std::unique_ptr<DatabaseConnection>& GetInstance(std::string_view connection_string);
    pqxx::result ExecQuery(std::string query, const pqxx::params& params);

};