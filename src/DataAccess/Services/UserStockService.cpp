#include "UserStockService.h"

#include <exception>

UserStockService::UserStockService(std::shared_ptr<UserStockRepositoryInterface>& repository)
    : repository_(repository)
{ }

UserStockService::UserStockService(UserStockService&& other)
    : repository_(other.repository_) 
{ }

UserStockService &UserStockService::operator=(UserStockService &&other){
    if (this == &other) {
        return *this;
    }

    repository_ = other.repository_;
    return *this;
}

ResultType<std::vector<UserStock>> UserStockService::GetUserStock(uint64_t user_id) {
    return repository_->ReadUserStocks(user_id);
}

ResultType<void> UserStockService::UserBuyStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) {
    return repository_->BuyStocks(amount, user_id, user_stock);
}

ResultType<void> UserStockService::UserSellStock(uint64_t amount, uint64_t user_id, const UserStock& user_stock) {
    return repository_->SellStocks(amount, user_id, user_stock);
}
