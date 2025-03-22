#pragma once

#include <memory>
#include <vector>

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

    void BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count);
    void SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count);
    void CreateNewUser(const User& user);
    void ChangePassword(uint64_t user_id, const std::string& password);
    std::vector<uint64_t> ListUsersStock(uint64_t user_id);
    
};