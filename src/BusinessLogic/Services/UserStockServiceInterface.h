#pragma once

#include <vector>
#include <cstdint>

#include "../Models/ResultType.h"
#include "../Models/Stock.h"
#include "../Models/UserStock.h"

class UserStockServiceInterface {
public:

    virtual ResultType<std::vector<UserStock>> GetUserStock(uint64_t user_id) = 0;
    virtual ResultType<void> UserBuyStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) = 0;
    virtual ResultType<void> UserSellStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) = 0;

};