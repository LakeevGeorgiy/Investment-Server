#pragma once

#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "../Models/User.h"

class UserRepositoryInterface {
public:

    virtual User CreateUser(const User& user) = 0;
    virtual std::vector<User> ReadUsers() = 0;
    virtual User ReadUser(uint64_t id) = 0;
    virtual bool FindUser(uint64_t id) = 0;
    virtual bool FindUserByLogin(const std::string& username) = 0;
    virtual User GetUserByLogin(const std::string& username) = 0;
    virtual void UpdateUser(const User& updated_user) = 0;
    virtual void DeleteUser(uint64_t id) = 0;
    virtual ~UserRepositoryInterface() = default;
};