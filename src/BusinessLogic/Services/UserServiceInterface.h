#pragma once

#include <memory>
#include <vector>

#include "../Models/User.h"
#include "../Models/ResultType.h"

class UserServiceInterface {
public:

    UserServiceInterface() = default;
    virtual ~UserServiceInterface() = default;

    virtual ResultType<User> GetUser(uint64_t id) = 0;
    virtual ResultType<uint64_t> CreateNewUser(const User& user) = 0;
    virtual ResultType<User> LoginUser(std::string_view email, std::string_view password) = 0;
    virtual ResultType<void> ChangePassword(uint64_t id, std::string_view password) = 0;
    
};