#include <iostream>
#include <boost/asio/buffer.hpp>
#include <algorithm>
#include <iterator>

#include "src/Presentation/Server.h"
#include "src/Presentation/Listener.h"

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

    const auto address = asio::ip::tcp::v4();
    const uint16_t port = 8080;
    auto doc_root = std::make_shared<const std::string>("/server");
    auto endpoint = asio::ip::tcp::endpoint(address, port);

    std::cout << "endpoint: " << endpoint << "\n";

    asio::io_context ctx(1);
    std::make_shared<Listener>(ctx, endpoint, doc_root)->Run();
    ctx.run();

    // for (int i = 0; i < 2; ++i) {
    //     auto result = server->ListStocks();
    //     result.wait();
    //     auto stocks = result.get();
    //     for (auto now : stocks) {
    //         std::cout << now.company_name_ << "\n";
    //     }
        
    // }
    // server->Join();
    // server->Stop();
    return 0;
}