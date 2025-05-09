#pragma once

#include "../../BusinessLogic/Repositories/UserRepositoryInterface.h"

class UserRepository : public UserRepositoryInterface {
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

    User CreateUser(const User& user) override;
    std::vector<User> ReadUsers() override;
    User ReadUser(uint64_t id) override;

    bool FindUser(uint64_t id) override;
    bool FindUserByLogin(const std::string& username) override;

    User GetUserByLogin(const std::string& username) override;
    void UpdateUser(const User& updated_user) override;
    void DeleteUser(uint64_t id) override;
};