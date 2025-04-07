#include <mutex>

#include "UserRepository.h"

UserRepository::UserRepository(): counter_(1), mutex_(), users_() {}

UserRepository::UserRepository(const UserRepository &other) {
    if (this == &other){
        return;
    }

    counter_ = other.counter_;
    users_ = other.users_;
}

void UserRepository::operator=(const UserRepository &other) {
    if (this == &other) {
        return;
    }

    counter_ = other.counter_;
    users_ = other.users_;
}

User UserRepository::CreateUser(const User &user)
{
    std::unique_lock lock(mutex_);
    auto ptr = std::shared_ptr<User>(new User(user));
    ptr->id_ = counter_;
    ptr->balance_ = user.balance_;
    users_[counter_++] = ptr;
    return *ptr;
}

std::vector<User> UserRepository::ReadUsers()
{
    std::shared_lock lock(mutex_);
    std::vector<User> result;
    for (auto& [key, value] : users_) {
        result.emplace_back(*value.get());
    }
    return std::move(result);
}

User UserRepository::ReadUser(uint64_t id) {
    return std::move(*users_[id].get());
}

bool UserRepository::FindUser(uint64_t id) {
    std::shared_lock lock(mutex_);
    return users_.find(id) != users_.end();
}

bool UserRepository::FindUserByLogin(const std::string &username){
    std::shared_lock lock(mutex_);
    for (auto& [id, cur_user] : users_) {
        if (cur_user->name_ == username) {
            return true;
        }
    }
    return false;
}

User UserRepository::GetUserByLogin(const std::string &username){
    std::shared_lock lock(mutex_);
    for (auto& [id, cur_user] : users_) {
        if (cur_user->name_ == username) {
            return *cur_user;
        }
    }
    return User(0, "", "",0);
}

void UserRepository::UpdateUser(const User &updated_user)
{
    std::unique_lock lock(mutex_);
    auto user = users_[updated_user.id_];
    user->name_ = updated_user.name_;
    user->password_ = updated_user.password_;
    user->balance_ = updated_user.balance_;
    user->stocks_ = updated_user.stocks_;
}

void UserRepository::DeleteUser(uint64_t id) {
    std::unique_lock lock(mutex_);
    users_.erase(id);
}