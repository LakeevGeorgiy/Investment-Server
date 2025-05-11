#include <iostream>
#include <boost/asio/buffer.hpp>
#include <algorithm>
#include <iterator>

#include "src/DataAccess/Repositories/StockRepository.h"
#include "src/DataAccess/Repositories/UserRepository.h"
#include "src/DataAccess/Repositories/DatabaseConnection.h"
#include "src/DataAccess/Services/StockService.h"
#include "src/DataAccess/Services/UserService.h"
#include "src/Presentation/TcpServer.h"

namespace asio = boost::asio;

void InsertStocks(std::shared_ptr<StockRepositoryInterface>& stock_repository_) {
    auto stocks_ = std::vector<Stock>{
        Stock(1, 2000, 1000, "Yandex"),
        Stock(2, 4500, 10'000, "Google"),
        Stock(3, 300, 1'000'000, "Sber"),
        Stock(4, 54000, 100, "Tesla"),
        Stock(5, 320, 10, "Ozon")
    };

    for (std::size_t i = 0; i < stocks_.size(); ++i) {
        stock_repository_->CreateStock(stocks_[i]);
    }
}

void InsertUsers(std::shared_ptr<UserRepositoryInterface>& user_repository_) {
    auto users_ = std::vector<User>{
        User(1, "Georgiy", "123", 1000000),
        User(2, "Vlad", "456", 1000000),
        User(3, "Andrew", "789", 1000000),
        User(4, "Achmed", "1011", 10000000),
        User(5, "Khabib", "1213", 1)
    };

    users_[0].stocks_[1] += 100;
    users_[0].stocks_[2] += 300;
    users_[0].stocks_[3] += 1000;

    for (std::size_t i = 0; i < users_.size(); ++i) {
        user_repository_->CreateUser(users_[i]);
    }
}

int main() {

    std::string_view connection_string = "postgresql://georgiy:postgres@localhost:5432/investments";
    auto& database = DatabaseConnection::GetInstance(connection_string);
    auto result = database->ExecQuery("select id from users where name = $1", pqxx::params{"Georgiy"});
    std::cout << result[0][0].as<int>() << "\n";
    
    std::shared_ptr<StockRepositoryInterface> stock_repository = std::make_shared<StockRepository>();
    std::shared_ptr<StockServiceInterface> stock_service = std::make_shared<StockService>(stock_repository);

    std::shared_ptr<UserRepositoryInterface> user_repository = std::make_shared<UserRepository>();
    std::shared_ptr<UserServiceInterface> user_service = std::make_shared<UserService>(user_repository);
    auto server = std::make_shared<Server>(user_service, stock_service);

    InsertStocks(stock_repository);
    InsertUsers(user_repository);

    const auto address = asio::ip::tcp::v4();
    asio::io_context ctx(1);
    TcpServer tcpServer(ctx, server);
    
    tcpServer.Run(address, 8080);
    return 0;
}