#pragma once

#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "../User.h"

class UserRepository {
public:

    using pointer = std::shared_ptr<User>;

private:

    uint64_t counter_;
    std::shared_mutex mutex_;
    std::unordered_map<uint64_t, pointer> users_;

public:

    UserRepository();
    UserRepository(const UserRepository& other);
    void operator=(const UserRepository& other);

    User CreateUser(const User& user);
    std::vector<User> ReadUsers();
    User ReadUser(uint64_t id);

    bool FindUser(uint64_t id);
    bool FindUserByLogin(const std::string& username);

    User GetUserByLogin(const std::string& username);
    void UpdateUser(const User& updated_user);
    void DeleteUser(uint64_t id);
};