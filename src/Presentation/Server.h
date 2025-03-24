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
    Server(const Server& other);
    void operator=(const Server& other);

    void ListStocks(std::function<void(std::vector<Stock>&)>& callback);
    void BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, std::function<void()>& callback);
    void SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count, std::function<void()>& callback);
    void Register(User& user, std::function<void()>& callback);
    void ChangePassword(uint64_t user_id, const std::string& password, std::function<void()>& callback);
    void ListUsersStock(uint64_t user_id, std::function<void(std::vector<Stock>&)>& callback);
    void Wait();
    void Join();
    void Stop();

    ~Server();
};