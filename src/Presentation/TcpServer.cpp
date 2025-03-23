#include "TcpServer.h"

TcpServer::TcpServer(asio::io_context& ctx, std::shared_ptr<Server>& server) : 
    ctx_(asio::io_context(1)), server_(server)
{}

void TcpServer::Run(const ip::tcp& address, const uint16_t port){
    auto endpoint = asio::ip::tcp::endpoint(address, port);
    auto doc_root = std::make_shared<const std::string>("/server");
    listener_ = std::make_shared<Listener>(ctx_, endpoint, server_, doc_root);
    listener_->Run();
    ctx_.run();
}
