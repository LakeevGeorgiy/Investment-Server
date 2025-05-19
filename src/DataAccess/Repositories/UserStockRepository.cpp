#include "UserStockRepository.h"

UserStockRepository::UserStockRepository(std::string_view connection_string)
    : mutex_()
    , db_(DatabaseConnection::GetInstance(connection_string))
{}

UserStockRepository::UserStockRepository(UserStockRepository &&other)
    : db_(other.db_) 
{ }

UserStockRepository &UserStockRepository::operator=(UserStockRepository&& other) {
    if (this == &other) {
        return *this;
    }
    db_ = std::move(other.db_);
    return *this;
}

ResultType<void> UserStockRepository::CreateUserStock(uint64_t user_id, const UserStock& user_stock){
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        INSERT INTO users_stocks (userId, stockId, count) 
        VALUES ($1, $2, $3) 
    )"};

    pqxx::params params{user_id, user_stock.stock_id_, user_stock.count_};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<uint64_t> UserStockRepository::GetCountStock(uint64_t user_id, uint64_t stock_id){
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT count
        FROM users_stocks
        WHERE userId = $1 AND stockId = $2
    )"};

    pqxx::params params{user_id, stock_id};
    auto result = db_->ExecQuery(query, params);
    uint64_t find = result[0][0].as<uint64_t>();
    return ResultType(find);
}

ResultType<std::vector<UserStock>> UserStockRepository::ReadUserStocks(uint64_t id){
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT stockId, count
        FROM users_stocks
        WHERE userId = $1
    )"};

    pqxx::params params{id};
    auto result = db_->ExecQuery(query, params);

    std::vector<UserStock> stocks;
    for (auto row : result) {
        std::optional<UserStock> user_stock = pqxx::string_traits<std::optional<UserStock>>::from_string(row);
        if (user_stock) {
            stocks.emplace_back(*user_stock);
        } else {
            return ResultType<std::vector<UserStock>>(
                std::make_shared<std::runtime_error>("There is no user with this id")
            );
        }
    }

    return ResultType(stocks);
}

ResultType<void> UserStockRepository::UpdateUserStock(uint64_t user_id, const UserStock& user_stock)
{
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        UPDATE users_stocks 
        SET count = $1
        WHERE userId = $2 AND stockId = $3;
    )"};

    pqxx::params params{user_stock.count_, user_id, user_stock.stock_id_};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<void> UserStockRepository::BuyStocks(uint64_t amount, uint64_t user_id, const UserStock &user_stock) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(

        UPDATE users_stocks
        SET count = count + $1
        WHERE userId = $2 AND stockId = $3;

        UPDATE stocks
        SET count = count - $1
        WHERE id = $3

        UPDATE users
        SET balance = balance - $4
        WHERE id = $2

    )"};

    pqxx::params params{user_stock.count_, user_id, user_stock.stock_id_, amount};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<void> UserStockRepository::SellStocks(uint64_t amount, uint64_t user_id, const UserStock &user_stock) {
        std::unique_lock lock(mutex_);

    std::string_view query = {R"(

        UPDATE users_stocks
        SET count = count - $1
        WHERE userId = $2 AND stockId = $3;

        UPDATE stocks
        SET count = count + $1
        WHERE id = $3

        UPDATE users
        SET balance = balance + $4
        WHERE id = $2

    )"};

    pqxx::params params{user_stock.count_, user_id, user_stock.stock_id_, amount};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<void> UserStockRepository::DeleteUserStock(uint64_t user_id, uint64_t stock_id) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        DELETE FROM users WHERE userId = $1 AND stockId = $2;
    )"};

    pqxx::params params{user_id, stock_id};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}
