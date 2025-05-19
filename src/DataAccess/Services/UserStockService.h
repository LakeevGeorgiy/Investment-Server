#pragma once

#include <memory>

#include "../../BusinessLogic/Repositories/UserStockRepositoryInterface.h"
#include "../../BusinessLogic/Services/UserStockServiceInterface.h"

class UserStockService : public UserStockServiceInterface {
private:

    std::shared_ptr<UserStockRepositoryInterface>& repository_;

public:

    UserStockService(std::shared_ptr<UserStockRepositoryInterface>& repository);
    UserStockService(const UserStockService& other) = delete;
    UserStockService(UserStockService&& other);
    UserStockService& operator=(const UserStockService& other) = delete;
    UserStockService& operator=(UserStockService&& other);

    ResultType<std::vector<UserStock>> GetUserStock(uint64_t user_id) override;
    ResultType<void> UserBuyStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) override;
    ResultType<void> UserSellStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) override;
};