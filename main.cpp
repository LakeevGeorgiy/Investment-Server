#include <iostream>
#include <boost/asio/buffer.hpp>
#include <algorithm>
#include <iterator>

#include "src/Presentation/Server.h"

namespace asio = boost::asio;

int main() {

    Stock stock(1, 2, 3, "yandex");
    Stock stock2(1,2,3, "google");
    auto stock_repository = std::make_shared<StockRepository>();
    stock_repository->CreateStock(stock);
    stock_repository->CreateStock(stock2);
    auto stock_service = std::make_shared<StockService>(stock_repository);
    auto user_repository = std::make_shared<UserRepository>();
    auto user_service = std::make_shared<UserService>(user_repository);
    auto server = std::make_shared<Server>(user_service, stock_service);


    for (int i = 0; i < 2; ++i) {
        auto result = server->ListStocks();
        result.wait();
        auto stocks = result.get();
        for (auto now : stocks) {
            std::cout << now.company_name_ << "\n";
        }
        
    }
    server->Join();
    server->Stop();
    return 0;
}