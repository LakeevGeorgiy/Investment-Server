#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include "Server.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

class Session : public std::enable_shared_from_this<Session> {
private:

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<Server> server_;
    std::shared_ptr<const std::string> doc_root_;
    http::request<http::string_body> req_;

public:

    Session(asio::ip::tcp::socket socket, std::shared_ptr<Server>& server, std::shared_ptr<const std::string>& doc_root);

    void Run();

    void GetStocksCallback(ResultType<std::vector<Stock>>& stocks);
    void GetIdCallback(ResultType<uint64_t>& result);
    void GetUserCallback(ResultType<User>& result);
    void GetVoidCallback(ResultType<void>& user_id);
    void BadRequestCallback(const std::string& message);

private:

    void DoRead();
    void OnRead(beast::error_code ec, std::size_t bytes_transfered);
    void SendResponse(http::message_generator&& msg);
    void OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred);
    void DoClose();

    void HandleRequest(
        beast::string_view doc_root, 
        http::request<http::string_body> &&req, 
        std::shared_ptr<Server>& server
    );
};


void to_json(nlohmann::json& j, const Stock& stock);
void from_json(nlohmann::json& j, Stock& stock);

void to_json(nlohmann::json& j, const User& stock);
void from_json(nlohmann::json& j, User& stock);