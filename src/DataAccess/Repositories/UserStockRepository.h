#pragma once

#include <memory>
#include <mutex>
#include <shared_mutex>

#include "../../BusinessLogic/Repositories/UserStockRepositoryInterface.h"
#include "DatabaseConnection.h"

class UserStockRepository : public UserStockRepositoryInterface {
private:

    std::shared_mutex mutex_;
    std::unique_ptr<DatabaseConnection>& db_;

public:

    UserStockRepository(std::string_view connection_string);
    UserStockRepository(const UserStockRepository& other) = delete;
    UserStockRepository(UserStockRepository&& other);
    UserStockRepository& operator=(const UserStockRepository& other) = delete;
    UserStockRepository& operator=(UserStockRepository&& other);

    ResultType<void> CreateUserStock(uint64_t user_id, const UserStock& user_stock) override;
    ResultType<uint64_t> GetCountStock(uint64_t user_id, uint64_t stock_id) override;
    ResultType<std::vector<UserStock>> ReadUserStocks(uint64_t id) override;
    ResultType<void> UpdateUserStock(uint64_t user_id, const UserStock& user_stock) override;
    ResultType<void> BuyStocks(uint64_t amount, uint64_t user_id, const UserStock& user_stock) override;
    ResultType<void> SellStocks(uint64_t amount, uint64_t user_id, const UserStock& user_stock) override;
    ResultType<void> DeleteUserStock(uint64_t user_id, uint64_t stock_id) override;
};