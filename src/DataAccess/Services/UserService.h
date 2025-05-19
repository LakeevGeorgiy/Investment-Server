#pragma once

#include "../../BusinessLogic/Repositories/UserRepositoryInterface.h"
#include "../../BusinessLogic/Services/UserServiceInterface.h"

class UserService : public UserServiceInterface {
private:

    std::shared_ptr<UserRepositoryInterface>& repository_;

public:

    UserService(std::shared_ptr<UserRepositoryInterface>& repository);
    UserService(const UserService& other) = delete;
    UserService(UserService&& other);
    UserService& operator=(const UserService& other) = delete;
    UserService& operator=(UserService&& other);
    ~UserService() = default;

    ResultType<User> GetUser(uint64_t id) override;
    ResultType<uint64_t> CreateNewUser(const User& user) override;
    ResultType<User> LoginUser(std::string_view email, std::string_view password) override;
    ResultType<void> ChangePassword(uint64_t id, std::string_view password) override;
    
};