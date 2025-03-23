#include "Session.h"

Session::Session(
    asio::ip::tcp::socket socket
    , std::shared_ptr<Server>& server
    , std::shared_ptr<const std::string>& doc_root
): 
    stream_(std::move(socket))
    , server_(server)
    , doc_root_(doc_root) 
{
    std::cout << "new session is created\n";
}

void Session::Run() {
    asio::dispatch(stream_.get_executor(), beast::bind_front_handler(
            &Session::DoRead, shared_from_this()
    ));
}

void Session::DoRead(){
    req_ = {};
    stream_.expires_after(std::chrono::seconds(30));
    http::async_read(stream_, buffer_, req_,
        beast::bind_front_handler(&Session::OnRead, shared_from_this())
    );
}

void Session::OnRead(beast::error_code ec, std::size_t bytes_transfered) {
    boost::ignore_unused(bytes_transfered);

    if (ec == http::error::end_of_stream) {
        return DoClose();
    }

    SendResponse(HandleRequest(*doc_root_, std::move(req_), server_));
}

void Session::SendResponse(http::message_generator &&msg){
    bool keep_alive = msg.keep_alive();

    beast::async_write(stream_, std::move(msg), beast::bind_front_handler(
        &Session::OnWrite, shared_from_this(), keep_alive
    ));
}

void Session::OnWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred){
    boost::ignore_unused(bytes_transferred);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    if (!keep_alive) {
        return DoClose();
    }

    DoRead();
}

void Session::DoClose(){
    beast::error_code ec;
    stream_.socket().shutdown(asio::ip::tcp::socket::shutdown_send, ec);
    std::cout << "connection is close\n";
}

http::message_generator HandleRequest(
    beast::string_view doc_root
    , http::request<http::string_body>&& req
    , std::shared_ptr<Server>& server)
{
    // std::string path = doc_root + req.target();
    http::response<http::string_body> response;
    std::cout << "req target: " << req.target() << "\n";

    if (req.target() == "/api/list_all_stocks" && req.method() == http::verb::get) {

        auto future_stocks = server->ListStocks();
        future_stocks.wait();
        auto stocks = future_stocks.get();
        nlohmann::json json_response(stocks);
        response.result(http::status::ok);
        response.version(11);
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "application/json");
        response.body() = json_response.dump();
        response.prepare_payload();
    } else if (req.target() == "/api/buy_stocks" && req.method() == http::verb::post){
        auto json_request = nlohmann::json::parse(req.body());
        std::string response_message = json_request.dump();
        std::cout << "request_message: " << json_request << "\n";
        response.result(http::status::ok);
        response.version(11);
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "text/html");
        response.body() = response_message;
        response.prepare_payload();
    } else {
        response.result(http::status::not_found);
        response.prepare_payload();
    }

    return response;
}

void to_json(nlohmann::json &j, const Stock &stock){
    j = nlohmann::json{
        {"id", stock.id_},
        {"cost", stock.cost_},
        {"count", stock.count_},
        {"company_name", stock.company_name_}
    };
}

void from_json(nlohmann::json &j, Stock &stock){
    j.at("id").get_to(stock.id_);
    j.at("cost").get_to(stock.cost_);
    j.at("count").get_to(stock.count_);
    j.at("company_name").get_to(stock.company_name_);
}

void to_json(nlohmann::json &j, const User &user){
    j = nlohmann::json{
        {"id", user.id_},
        {"name", user.name_},
        {"password", user.password_},
        {"stocks", user.stocks_},
    };
}

void from_json(nlohmann::json &j, User &user){
    j.at("id").get_to(user.id_);
    j.at("name").get_to(user.name_);
    j.at("password").get_to(user.password_);
    j.at("stocks").get_to(user.stocks_);
}