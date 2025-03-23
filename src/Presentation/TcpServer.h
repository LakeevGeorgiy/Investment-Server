#pragma once

#include "Server.h"
#include "Listener.h"

namespace asio = boost::asio;

class TcpServer {
private:

    asio::io_context ctx_;
    std::shared_ptr<Server> server_;
    std::shared_ptr<Listener> listener_;

public:

    TcpServer(asio::io_context& ctx, std::shared_ptr<Server>& server);

    void Run(const ip::tcp& address, const uint16_t port);
};