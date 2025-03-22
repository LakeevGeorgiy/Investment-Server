#include "UserService.h"

UserService::UserService(pointer<UserRepository>& user_repository): user_repository_(user_repository){}

UserService::UserService(const UserService& other) {
    if (this == &other) {
        return;
    }

    user_repository_ = other.user_repository_;
}

void UserService::operator=(const UserService &other){
    if (this == &other) {
        return;
    }

    user_repository_ = other.user_repository_;
}

void UserService::BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count)
{
    User updated_user = user_repository_->ReadUser(user_id);
    updated_user.stocks_[stock_id] += count;
    user_repository_->UpdateUser(updated_user);
}

void UserService::SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count) {
    User updated_user = user_repository_->ReadUser(user_id);
    if (updated_user.stocks_.find(stock_id) == updated_user.stocks_.end()
            || updated_user.stocks_[stock_id] < count) {
        throw std::exception();
    }
    updated_user.stocks_[stock_id] -= count;
    if (updated_user.stocks_[stock_id] == 0) {
        updated_user.stocks_.erase(stock_id);
    }
    user_repository_->UpdateUser(updated_user);
}

void UserService::CreateNewUser(const User &user) {
    user_repository_->CreateUser(user);
}

void UserService::ChangePassword(uint64_t user_id, const std::string &password){
    User updated_user = user_repository_->ReadUser(user_id);
    updated_user.password_ = password;
    user_repository_->UpdateUser(updated_user);
}

std::vector<uint64_t> UserService::ListUsersStock(uint64_t user_id) {
    User user = user_repository_->ReadUser(user_id);
    std::vector<uint64_t> stocks_id;
    for (auto& [key, value]: user.stocks_) {
        stocks_id.emplace_back(key);
    }
    return stocks_id;
}
