#pragma once

#include <boost/asio/thread_pool.hpp>
#include <future>

#include "../BusinessLogic/StockService.h"
#include "../BusinessLogic/UserService.h"

class Server : public std::enable_shared_from_this<Server>{
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    boost::asio::thread_pool pool_;
    pointer<UserService> user_service_;
    pointer<StockService> stock_service_;

public:

    Server(pointer<UserService>& user_service, pointer<StockService>& stock_service);
    Server(const Server& other) = delete;
    void operator=(const Server& other);

    std::future<std::vector<Stock>> ListStocks();
    std::future<void> BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count);
    std::future<void> SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count);
    std::future<void> Register(const User& user);
    std::future<void> ChangePassword(uint64_t user_id, const std::string& password);
    std::future<std::vector<Stock>> ListUsersStock(uint64_t user_id);
    void Wait();
    void Join();
    void Stop();

    ~Server();
};