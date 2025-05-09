#pragma once

#include <boost/asio/thread_pool.hpp>
#include <future>

#include "../BusinessLogic/Services/StockServiceInterface.h"
#include "../BusinessLogic/Services/UserServiceInterface.h"

class Server : public std::enable_shared_from_this<Server>{
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    boost::asio::thread_pool pool_;
    pointer<UserServiceInterface> user_service_;
    pointer<StockServiceInterface> stock_service_;

public:

    Server(pointer<UserServiceInterface>& user_service, pointer<StockServiceInterface>& stock_service);
    Server(const Server& other);
    void operator=(const Server& other);

    void ListStocks(std::function<void(std::vector<Stock>&)>& callback);
    void BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, std::function<void(ResultType<uint64_t>&)>& callback);
    void SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, std::function<void(ResultType<uint64_t>&)>& callback);
    void Register(User& user, std::function<void(ResultType<User>&)>& callback);
    void Login(User& user, std::function<void(ResultType<User>& user)>& callback);
    void ChangePassword(uint64_t user_id, const std::string& password, std::function<void()>& callback);
    void ListUsersStock(uint64_t user_id, std::function<void(ResultType<std::vector<Stock>>)>& callback);
    void BadRequest(std::function<void()>& callback);
    void Wait();
    void Join();
    void Stop();

    ~Server();
};