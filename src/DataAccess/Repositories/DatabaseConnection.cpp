#include "DatabaseConnection.h"

#include <iostream>

std::unique_ptr<DatabaseConnection> DatabaseConnection::db_ = nullptr;

DatabaseConnection::DatabaseConnection(std::string_view connection_string)
    : conn_(connection_string.data())
{ }

std::unique_ptr<DatabaseConnection>& DatabaseConnection::GetInstance(std::string_view connection_string) {
    if (DatabaseConnection::db_.get() == nullptr) {
        DatabaseConnection::db_ = std::unique_ptr<DatabaseConnection>(new DatabaseConnection(connection_string));
    }
    return DatabaseConnection::db_;
}

pqxx::result DatabaseConnection::ExecQuery(std::string_view query, const pqxx::params& params){
    pqxx::work transaction(conn_);
    pqxx::result r = transaction.exec(query, params);
    transaction.commit();
    return r;
}