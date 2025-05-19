#pragma once

#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "../Models/User.h"
#include "../Models/ResultType.h"

class UserRepositoryInterface {
public:

    virtual ResultType<uint64_t> CreateUser(const User& user) = 0;
    virtual ResultType<User> ReadUser(uint64_t id) = 0;
    virtual ResultType<User> LoginUser(std::string_view email, std::string_view password) = 0;
    virtual ResultType<void> UpdateUser(const User& user) = 0;
    virtual ResultType<void> DeleteUser(uint64_t id) = 0;
    virtual ~UserRepositoryInterface() = default;
};