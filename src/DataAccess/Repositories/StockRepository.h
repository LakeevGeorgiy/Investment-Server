#pragma once

#include <shared_mutex>
#include <unordered_map>
#include <memory>

#include "../../BusinessLogic/Repositories/StockRepositoryInterface.h"
#include "DatabaseConnection.h"

class StockRepository : public StockRepositoryInterface {
private:

    std::shared_mutex mutex_;
    std::unique_ptr<DatabaseConnection>& db_;

public:

    StockRepository(std::string_view connection_string);
    StockRepository(const StockRepository& other) = delete;
    StockRepository(StockRepository&& other);
    StockRepository& operator=(const StockRepository& other) = delete;
    StockRepository& operator=(StockRepository&& other);

    ResultType<uint64_t> CreateStock(const Stock& stock) override;
    ResultType<std::vector<Stock>> ReadStocks() override;
    ResultType<Stock> ReadStock(uint64_t id) override;
    ResultType<void> UpdateStock(const Stock& stock) override;
    ResultType<void> DeleteStock(uint64_t id) override;
};