#include <stdexcept>

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

ResultType<User> UserService::GetUser(uint64_t user_id){
    if (!user_repository_->FindUser(user_id)) {
        return ResultType<User>(
            std::make_shared<std::runtime_error>("no such user"));
    }

    User user = user_repository_->ReadUser(user_id);
    return ResultType(user);
}

ResultType<uint64_t> UserService::BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost)
{
    if (!user_repository_->FindUser(user_id)) {
        return ResultType<uint64_t>(
            std::make_shared<std::runtime_error>("no such user"));
    }

    User updated_user = user_repository_->ReadUser(user_id);
    updated_user.balance_ -= cost * count;
    updated_user.stocks_[stock_id] += count;
    user_repository_->UpdateUser(updated_user);
    return ResultType(updated_user.balance_);
}

ResultType<uint64_t> UserService::SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost) {
    User updated_user = user_repository_->ReadUser(user_id);

    if (updated_user.stocks_.find(stock_id) == updated_user.stocks_.end()){
        return ResultType<uint64_t>(
            std::make_shared<std::runtime_error>("user doesn't have such stocks"));
    }

    if (updated_user.stocks_[stock_id] < count) {
        return ResultType<uint64_t>(
            std::make_shared<std::runtime_error>("Try to sell more than have"));
    }

    updated_user.balance_ += cost * count;
    updated_user.stocks_[stock_id] -= count;
    if (updated_user.stocks_[stock_id] == 0) {
        updated_user.stocks_.erase(stock_id);
    }
    user_repository_->UpdateUser(updated_user);
    return ResultType<uint64_t>(updated_user.balance_);
}

ResultType<User> UserService::CreateNewUser(const User &user) {
    if (user_repository_->FindUserByLogin(user.name_)) {
        return ResultType<User>(
            std::make_shared<std::runtime_error>("user already exist"));
    }
    auto new_user = user_repository_->CreateUser(user);
    return ResultType(new_user);
}

ResultType<User> UserService::LoginUser(User &user) {
    if (!user_repository_->FindUserByLogin(user.name_)) {
        return ResultType<User>(
            std::make_shared<std::runtime_error>("no such user"));
    }

    auto find_user = user_repository_->GetUserByLogin(user.name_);
    if (find_user.password_ == user.password_) {
        return ResultType(find_user);
    }
    return ResultType<User>(
        std::make_shared<std::runtime_error>("password isn't correct"));
}

void UserService::ChangePassword(uint64_t user_id, const std::string &password){
    User updated_user = user_repository_->ReadUser(user_id);
    updated_user.password_ = password;
    user_repository_->UpdateUser(updated_user);
}

ResultType<std::vector<std::pair<uint64_t, uint32_t>>> UserService::ListUsersStock(uint64_t user_id) {
    if (!user_repository_->FindUser(user_id)){
        return ResultType<std::vector<std::pair<uint64_t, uint32_t>>>(
            std::make_shared<std::runtime_error>("No such user"));
    }
    User user = user_repository_->ReadUser(user_id);
    std::vector<std::pair<uint64_t, uint32_t>> stocks_id;
    for (auto& [key, value]: user.stocks_) {
        stocks_id.emplace_back(key, value);
    }
    return ResultType<std::vector<std::pair<uint64_t, uint32_t>>>(stocks_id);
}
