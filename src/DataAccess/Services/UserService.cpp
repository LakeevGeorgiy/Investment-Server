#include <stdexcept>

#include "UserService.h"

UserService::UserService(std::shared_ptr<UserRepositoryInterface>& repository)
    : repository_(repository) { }

UserService::UserService(UserService&& other)
    : repository_(other.repository_) {}

UserService& UserService::operator=(UserService&& other){
    if (this == &other) {
        return *this;
    }

    repository_ = std::move(other.repository_);
    return *this;
}

ResultType<User> UserService::GetUser(uint64_t id){
    return repository_->ReadUser(id);
}

ResultType<uint64_t> UserService::CreateNewUser(const User& user) {
    return repository_->CreateUser(user);
}

ResultType<User> UserService::LoginUser(std::string_view email, std::string_view password) {
   return repository_->LoginUser(email, password);
}

ResultType<void> UserService::ChangePassword(uint64_t id, std::string_view password){
    auto result_user = repository_->ReadUser(id);
    if (!result_user.IsSuccess()) {
        return ResultType<void>(
            std::make_shared<std::runtime_error>("No such user")
        );
    }

    auto user = result_user.GetResult();
    user.password_ = password;
    return repository_->UpdateUser(user);
}