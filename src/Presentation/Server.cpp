#include <boost/asio/post.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/packaged_task.hpp>
#include <chrono>
#include <thread>
#include <iostream>

#include "Server.h"

Server::Server(
    pointer<UserServiceInterface>& user_service
    , pointer<StockServiceInterface>& stock_service
    , pointer<UserStockServiceInterface>& user_stock_service
)
    : user_service_(user_service)
    , stock_service_(stock_service)
    , user_stock_service_(user_stock_service)
{ }

Server::Server(const Server &other)
    : user_service_(other.user_service_)
    , stock_service_(other.stock_service_)
    , user_stock_service_(other.user_stock_service_)
{ }

void Server::operator=(const Server &other){
    if (this == &other) {
        return;
    }

    user_service_ = other.user_service_;
    stock_service_ = other.stock_service_;
    user_stock_service_ = other.user_stock_service_;
}

void Server::ListStocks(std::function<void(ResultType<std::vector<Stock>>&)>& callback) {
    boost::asio::post(pool_, 
        [callback, server = shared_from_this()](){
            auto result = server->stock_service_->ListStocks();
            callback(result);
        }
    );
}

void Server::BuyStocks(uint64_t user_id, const UserStock& user_stock,
                       std::function<void(ResultType<void> &)> &callback)
{

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), user_id, user_stock](){
            auto result_stock = server->stock_service_->GetStock(user_stock.stock_id_);
            if (!result_stock.IsSuccess()) {
                auto error = ResultType<void>(result_stock.GetError());
                callback(error);
                return;
            }
            auto cost = result_stock.GetResult().cost_;

            auto result_user = server->user_service_->GetUser(user_id);
            if (!result_user.IsSuccess()) {
                auto error = ResultType<void>(result_user.GetError());
                callback(error);
                return;
            }
            auto user = result_user.GetResult();
            if (user.balance_ < cost) {
                auto error = ResultType<void>(
                    std::make_shared<std::runtime_error>("Account has insufficient funds")
                );
                callback(error);
                return;
            }

            auto result = server->user_stock_service_->UserBuyStock(cost * user_stock.count_, user_id, user_stock);
            callback(result);
        }
    );
}

void Server::SellStocks(uint64_t user_id, const UserStock& user_stock,
                                    std::function<void(ResultType<void>&)>& callback) {

    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id, user_stock](){

            auto result_stock = server->stock_service_->GetStock(user_stock.stock_id_);
            if (!result_stock.IsSuccess()) {
                auto error = ResultType<void>(result_stock.GetError());
                callback(error);
                return;
            }
            auto cost = result_stock.GetResult().cost_;

            auto result = server->user_stock_service_->UserSellStock(user_stock.count_ * cost, user_id, user_stock);
            callback(result);
        }
    );

}

void Server::Register(User& user, std::function<void(ResultType<uint64_t>&)>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), &user] {
            auto new_user = server->user_service_->CreateNewUser(user);
            callback(new_user);
        }
    );

}

void Server::Login(std::string_view email, std::string_view password, std::function<void(ResultType<User>& user)>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), email, password] {
            auto auth_user = server->user_service_->LoginUser(email, password);
            callback(auth_user);
        }
    );

}

void Server::ChangePassword(uint64_t user_id, std::string_view password, std::function<void(ResultType<void>&)>& callback){

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), user_id, password]{
            auto result = server->user_service_->ChangePassword(user_id, password);
            callback(result);
        }
    );

}

void Server::ListUsersStock(uint64_t user_id, std::function<void(ResultType<std::vector<Stock>>)>& callback) {
    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id](){
            auto result_id = server->user_stock_service_->GetUserStock(user_id);
            if (!result_id.IsSuccess()){
                ResultType<std::vector<Stock>> res(result_id.GetError());
                callback(res);
                return;
            }

            auto stock_id = result_id.GetResult();
            std::vector<Stock> stocks;

            for (auto& cur : stock_id) {
                auto cur_stock = server->stock_service_->GetStock(cur.stock_id_);
                if (!cur_stock.IsSuccess()) {
                    callback(ResultType(stocks));
                    return;
                }
                stocks.emplace_back(cur_stock.GetResult());
                stocks.back().count_ = cur.count_;
            }
            callback(ResultType(stocks));
        }
    );
}

void Server::BadRequest(std::function<void()> &callback){
    boost::asio::post(pool_, 
        [callback, server = shared_from_this()](){
            callback();
        }
    );
}

void Server::Wait(){
    pool_.wait();
}

void Server::Join(){
    pool_.join();
}

void Server::Stop(){
    pool_.stop();
}

Server::~Server() {
    pool_.stop();
}
