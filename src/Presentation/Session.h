#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

class Session : public std::enable_shared_from_this<Session> {
private:

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<const std::string> doc_root_;
    http::request<http::string_body> req_;

public:

    Session(asio::ip::tcp::socket socket, std::shared_ptr<const std::string>& doc_root);

    void Run();

private:

    void DoRead();
    void OnRead(beast::error_code ec, std::size_t bytes_transfered);
    void SendResponse(http::message_generator&& msg);
    void OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred);
    void DoClose();

};

http::message_generator HandleRequest(
    beast::string_view doc_root, http::request<http::string_body> &&req);