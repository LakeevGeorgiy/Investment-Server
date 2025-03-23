#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>

#include "Session.h"
#include "Server.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ip = asio::ip;

class Listener : public std::enable_shared_from_this<Listener> {
private:

    asio::io_context& context_;
    ip::tcp::acceptor acceptor_;
    std::shared_ptr<Server> server_;
    std::shared_ptr<const std::string> doc_root_;

public:

    Listener(
        asio::io_context& context,
        ip::tcp::endpoint endpoint,
        std::shared_ptr<Server>& server,
        std::shared_ptr<const std::string>& doc_root
    );

    void Run();

private:

    void DoAccept();
    void OnAccept(beast::error_code ec, ip::tcp::socket socket);
};