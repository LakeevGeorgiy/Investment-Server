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
            std::cout << "list stocks start\n";
            auto result = server->stock_service_->ListStocks();
            // auto result = std::vector<Stock>();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cout << "list stocks done\n";
            callback(result);
        }
    );
}

void Server::BuyStocks(uint64_t user_id, uint64_t stock_id, uint32_t count,
                                    std::function<void()>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), user_id, stock_id, count](){
            server->stock_service_->BuyStocks(stock_id, count);
            server->user_service_->BuyStocks(user_id, stock_id, count);
            callback();
        }
    );

}

void Server::SellStocks(uint64_t user_id, uint64_t stock_id, uint32_t count,
                                    std::function<void()>& callback) {

    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id, stock_id, count](){
            server->stock_service_->SellStocks(stock_id, count);
            server->user_service_->SellStocks(user_id, stock_id, count);
            callback();
        }
    );

}

void Server::Register(User& user, std::function<void()>& callback) {

    boost::asio::post(pool_,
        [callback, server = shared_from_this(), &user] {
            server->user_service_->CreateNewUser(user);
            callback();
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

void Server::ListUsersStock(uint64_t user_id, std::function<void(std::vector<Stock>&)>& callback) {
    boost::asio::post(pool_, 
        [callback, server = shared_from_this(), user_id](){
            auto result = server->user_service_->ListUsersStock(user_id);
            std::vector<Stock> stocks;
            for (int i = 0; i < result.size(); ++i) {
                stocks.emplace_back(server->stock_service_->GetStock(result[i]));
            }
            callback(stocks);
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
