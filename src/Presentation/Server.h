#pragma once

#include <boost/asio/thread_pool.hpp>
#include <future>

#include "../BusinessLogic/Services/StockServiceInterface.h"
#include "../BusinessLogic/Services/UserServiceInterface.h"
#include "../BusinessLogic/Services/UserStockServiceInterface.h"

class Server : public std::enable_shared_from_this<Server>{
public:

    template <typename T>
    using pointer = std::shared_ptr<T>;

private:

    boost::asio::thread_pool pool_;
    pointer<UserServiceInterface> user_service_;
    pointer<StockServiceInterface> stock_service_;
    pointer<UserStockServiceInterface>& user_stock_service_;

public:

    Server(pointer<UserServiceInterface>& user_service
        , pointer<StockServiceInterface>& stock_service
        , pointer<UserStockServiceInterface>& user_stock_service);
    Server(const Server& other);
    void operator=(const Server& other);

    void ListStocks(std::function<void(ResultType<std::vector<Stock>>&)>& callback);
    void BuyStocks(uint64_t user_id, const UserStock& user_stock, std::function<void(ResultType<void>&)>& callback);
    void SellStocks(uint64_t user_id, const UserStock& user_stock, std::function<void(ResultType<void>&)>& callback);
    void Register(User& user, std::function<void(ResultType<uint64_t>&)>& callback);
    void Login(std::string_view email, std::string_view password, std::function<void(ResultType<User>& user)>& callback);
    void ChangePassword(uint64_t user_id, std::string_view password, std::function<void(ResultType<void>&)>& callback);
    void ListUsersStock(uint64_t user_id, std::function<void(ResultType<std::vector<Stock>>)>& callback);
    void BadRequest(std::function<void()>& callback);
    void Wait();
    void Join();
    void Stop();

    ~Server();
};