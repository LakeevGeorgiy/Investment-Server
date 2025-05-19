#pragma once

#include <vector>

#include "../Models/Stock.h"
#include "../Models/ResultType.h"
#include "../Models/UserStock.h"

class UserStockRepositoryInterface {
public:

    virtual ResultType<void> CreateUserStock(uint64_t user_id, const UserStock& user_stock) = 0;
    virtual ResultType<uint64_t> GetCountStock(uint64_t user_id, uint64_t stock_id) = 0;
    virtual ResultType<std::vector<UserStock>> ReadUserStocks(uint64_t id) = 0;
    virtual ResultType<void> UpdateUserStock(uint64_t user_id, const UserStock& user_stock) = 0;
    virtual ResultType<void> BuyStocks(uint64_t amount, uint64_t user_id, const UserStock& user_stock) = 0;
    virtual ResultType<void> SellStocks(uint64_t amount, uint64_t user_id, const UserStock& user_stock) = 0;
    virtual ResultType<void> DeleteUserStock(uint64_t user_id, uint64_t stock_id) = 0;
    
};