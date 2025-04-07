#pragma once

#include <memory>
#include <vector>

#include "../DataAccess/ResultType.h"
#include "../DataAccess/Repositories/UserRepository.h"

class UserService {
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    pointer<UserRepository> user_repository_;

public:

    UserService(pointer<UserRepository>& user_repository);
    UserService(const UserService& other);
    void operator=(const UserService& other);

    ResultType<User> GetUser(uint64_t user_id);
    ResultType<uint64_t> BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost);
    ResultType<uint64_t> SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost);
    ResultType<User> CreateNewUser(const User& user);
    ResultType<User> LoginUser(User& user);
    void ChangePassword(uint64_t user_id, const std::string& password);
    ResultType<std::vector<std::pair<uint64_t, uint32_t>>> ListUsersStock(uint64_t user_id);
    
};