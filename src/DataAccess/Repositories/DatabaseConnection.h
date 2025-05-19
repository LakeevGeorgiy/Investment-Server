#pragma once

#include <pqxx/pqxx>

#include "../../BusinessLogic/Models/User.h"
#include "../../BusinessLogic/Models/Stock.h"
#include "../../BusinessLogic/Models/UserStock.h"
#include "../../BusinessLogic/Repositories/DatabaseConnectionInterface.h"

class DatabaseConnection : public DatabaseConnectionInterface {
private:

    pqxx::connection conn_;
    static std::unique_ptr<DatabaseConnection> db_;

    DatabaseConnection(std::string_view connection_string);

public:

    static std::unique_ptr<DatabaseConnection>& GetInstance(std::string_view connection_string);
    pqxx::result ExecQuery(std::string_view query, const pqxx::params& params);

};

namespace pqxx {

    template<>
    struct string_traits<std::optional<User>> {
        static std::optional<User> from_string(const pqxx::row& row) {
            if (row["id"].is_null()) {
                return std::nullopt;
            }
            return User{
                row["id"].as<uint64_t>(),
                row["name"].as<std::string_view>(),
                row["email"].as<std::string_view>(),
                row["password"].as<std::string_view>(),
                row["balance"].as<uint64_t>()
            };
        }
    };

    template<>
    struct string_traits<std::optional<Stock>> {
        static std::optional<Stock> from_string(const pqxx::row& row) {
            if (row["id"].is_null()) {
                return std::nullopt;
            }
            return Stock{
                row["id"].as<uint64_t>(),
                row["cost"].as<uint32_t>(),
                row["count"].as<uint32_t>(),
                row["company"].as<std::string>()
            };
        }
    };

    template<>
    struct string_traits<std::optional<UserStock>> {
        static std::optional<UserStock> from_string(const pqxx::row& row) {
            return UserStock{
                row["stockid"].as<uint64_t>(),
                row["count"].as<uint32_t>(),
            };
        }
    };
}