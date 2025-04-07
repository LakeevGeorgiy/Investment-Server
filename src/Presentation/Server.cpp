#include <boost/asio/post.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/packaged_task.hpp>
#include <chrono>
#include <thread>

#include "Server.h"

Server::Server(pointer<UserService>& user_service, pointer<StockService>& stock_service): 
        user_service_(user_service), stock_service_(stock_service){ }

Server::Server(const Server &other):
    user_service_(other.user_service_), stock_service_(other.stock_service_){}

void Server::operator=(const Server &other){
    if (this == &other) {
        return;
    }

    user_service_ = other.user_service_;
    stock_service_ = other.stock_service_;
}

void Server::ListStocks(std::function<void(std::vector<Stock>&)>& callback) {
    boost::asio::post(pool_, 
        [callback, server = shared_from_this()](){
            auto result = server->stock_service_->ListStocks();
            callback(result);
        }
    );
}

void Server::BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count,
                       std::function<void(ResultType<void> &)> &callback)
{

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), user_id, stock_id, count](){
            auto result = server->stock_service_->BuyStocks(stock_id, count);
            if (!result.IsSuccess()) {
                callback(result);
                return;
            }

            result = server->user_service_->BuyStocks(user_id, stock_id, count);
            callback(result);
        }
    );
}

void Server::SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count,
                                    std::function<void(ResultType<void>&)>& callback) {

    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id, stock_id, count](){
            auto result = server->stock_service_->SellStocks(stock_id, count);
            if (!result.IsSuccess()) {
                callback(result);
            }

            result = server->user_service_->SellStocks(user_id, stock_id, count);
            callback(result);
        }
    );

}

void Server::Register(User& user, std::function<void(ResultType<User>&)>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), &user] {
            auto new_user = server->user_service_->CreateNewUser(user);
            callback(new_user);
        }
    );

}

void Server::Login(User& user, std::function<void(ResultType<User>& user)>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), &user] {
            auto auth_user = server->user_service_->LoginUser(user);
            callback(auth_user);
        }
    );

}

void Server::ChangePassword(uint64_t user_id, const std::string &password, std::function<void()>& callback){

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), user_id, &password]{
            server->user_service_->ChangePassword(user_id, password);
            callback();
        }
    );

}

void Server::ListUsersStock(uint64_t user_id, std::function<void(ResultType<std::vector<Stock>>)>& callback) {
    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id](){
            auto result = server->user_service_->ListUsersStock(user_id);
            std::cout << "list user stock: before if\n";
            if (!result.IsSuccess()){
                ResultType<std::vector<Stock>> res(result.GetError());
                callback(res);
                return;
            }
            std::cout << "list user stock: after if\n";
            std::vector<Stock> stocks;
            for (auto& [id, cnt] : result.GetResult()) {
                auto cur_stock = server->stock_service_->GetStock(id);
                if (!cur_stock.IsSuccess()) {
                    callback(ResultType(stocks));
                    return;
                }
                stocks.emplace_back(cur_stock.GetResult());
                stocks.back().count_ = cnt;
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
