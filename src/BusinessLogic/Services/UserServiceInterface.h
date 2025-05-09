#pragma once

#include <memory>
#include <vector>

#include "../../BusinessLogic/Models/ResultType.h"
#include "../../BusinessLogic/Repositories/UserRepositoryInterface.h"

class UserServiceInterface {
public:

    virtual ResultType<User> GetUser(uint64_t user_id) = 0;
    virtual ResultType<uint64_t> BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost) = 0;
    virtual ResultType<uint64_t> SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost) = 0;
    virtual ResultType<User> CreateNewUser(const User& user) = 0;
    virtual ResultType<User> LoginUser(User& user) = 0;
    virtual void ChangePassword(uint64_t user_id, const std::string& password) = 0;
    virtual ResultType<std::vector<std::pair<uint64_t, uint32_t>>> ListUsersStock(uint64_t user_id) = 0;
    
};