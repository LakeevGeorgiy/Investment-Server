#pragma once

#include "../../BusinessLogic/Services/UserServiceInterface.h"

class UserService : public UserServiceInterface {
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    pointer<UserRepositoryInterface> user_repository_;

public:

    UserService(pointer<UserRepositoryInterface>& user_repository);
    UserService(const UserService& other);
    void operator=(const UserService& other);

    ResultType<User> GetUser(uint64_t user_id) override;
    ResultType<uint64_t> BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost) override;
    ResultType<uint64_t> SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, uint64_t cost) override;
    ResultType<User> CreateNewUser(const User& user) override;
    ResultType<User> LoginUser(User& user) override;
    void ChangePassword(uint64_t user_id, const std::string& password) override;
    ResultType<std::vector<std::pair<uint64_t, uint32_t>>> ListUsersStock(uint64_t user_id) override;
    
};