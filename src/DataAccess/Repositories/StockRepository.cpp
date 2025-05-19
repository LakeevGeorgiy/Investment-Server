#include <mutex>
#include <optional>

#include "StockRepository.h"

StockRepository::StockRepository(std::string_view connection_string)
    : mutex_()
    , db_(DatabaseConnection::GetInstance(connection_string))
{}

StockRepository::StockRepository(StockRepository&& other)
    : db_(other.db_) 
{}

StockRepository& StockRepository::operator=(StockRepository&& other) {
    if (this == &other) {
        return *this;
    }
    db_ = std::move(other.db_);
    return *this;
}

ResultType<uint64_t> StockRepository::CreateStock(const Stock& stock) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        INSERT INTO stocks (cost, count, company) 
        VALUES ($1, $2, $3) 
        RETURNING id;
    )"};

    pqxx::params params{stock.cost_, stock.count_, stock.company_name_};
    auto result = db_->ExecQuery(query, params);
    uint64_t id = result[0][0].as<uint64_t>();
    return ResultType<uint64_t>(id);
}

ResultType<std::vector<Stock>> StockRepository::ReadStocks() {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT id, cost, count, company
        FROM stocks;
    )"};

    auto result = db_->ExecQuery(query, pqxx::params{});
    std::vector<Stock> stocks;
    for (auto row : result) {
        std::optional<Stock> stock = pqxx::string_traits<std::optional<Stock>>::from_string(row);
        if (stock) {
            stocks.emplace_back(*stock);
        }
    }

    return ResultType(stocks);
}

ResultType<Stock> StockRepository::ReadStock(uint64_t id) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        SELECT id, cost, count, company
        FROM stocks
        WHERE id = $1;
    )"};

    pqxx::params params { id };
    auto result = db_->ExecQuery(query, params);
    std::optional<Stock> stock = pqxx::string_traits<std::optional<Stock>>::from_string(result[0]);
    if (stock) {
        return ResultType(*stock);
    }
    return ResultType<Stock>(
        std::make_shared<std::runtime_error>("There is not stock with this id")
    );
}

ResultType<void> StockRepository::UpdateStock(const Stock& stock) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        UPDATE stocks 
        SET cost = $1, count = $2, company = $3
        WHERE id = $4;
    )"};

    pqxx::params params{ stock.cost_, stock.count_, stock.company_name_};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}

ResultType<void> StockRepository::DeleteStock(uint64_t id) {
    std::unique_lock lock(mutex_);

    std::string_view query = {R"(
        DELETE FROM stocks WHERE id = $1;
    )"};

    pqxx::params params{id};
    db_->ExecQuery(query, params);
    return ResultType<void>();
}