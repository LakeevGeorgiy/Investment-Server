#pragma once

#include "../../BusinessLogic/Repositories/UserRepositoryInterface.h"
#include "DatabaseConnection.h"

class UserRepository : public UserRepositoryInterface {
private:

    std::shared_mutex mutex_;
    std::unique_ptr<DatabaseConnection>& db_;

public:

    UserRepository(std::string_view connection_string);
    UserRepository(const UserRepository& other) = delete;
    UserRepository(UserRepository&& other);
    UserRepository& operator=(const UserRepository& other) = delete;
    UserRepository& operator=(UserRepository&& other);

    ResultType<uint64_t> CreateUser(const User& user) override;
    ResultType<User> ReadUser(uint64_t id) override;
    ResultType<User> LoginUser(std::string_view email, std::string_view password) override;
    ResultType<void> UpdateUser(const User& user) override;
    ResultType<void> DeleteUser(uint64_t id) override;
};