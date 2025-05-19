#include "Session.h"

Session::Session(
    asio::ip::tcp::socket socket
    , std::shared_ptr<Server>& server
    , std::shared_ptr<const std::string>& doc_root
): 
    stream_(std::move(socket))
    , server_(server)
    , doc_root_(doc_root) 
{}

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

    HandleRequest(*doc_root_, std::move(req_), server_);
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
}

void Session::GetStocksCallback(ResultType<std::vector<Stock>>& result_stocks) {

    if (!result_stocks.IsSuccess()) {
        BadRequestCallback(result_stocks.GetError()->what());
        return;
    }

    auto stocks = result_stocks.GetResult();
    http::response<http::string_body> response;
    nlohmann::json json_response(stocks);

    response.result(http::status::ok);
    response.version(11);
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");
    response.body() = json_response.dump();
    response.prepare_payload();

    http::message_generator generator(std::move(response));
    SendResponse(std::move(generator));
}

void Session::GetIdCallback(ResultType<uint64_t>& result) {

    if (!result.IsSuccess()) {
        BadRequestCallback(result.GetError()->what());
        return;
    }

    http::response<http::string_body> response;
    response.version(11);
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    auto balance = result.GetResult();
    nlohmann::json json_response{
        {"balance", balance}
    };
    response.result(http::status::ok);
    response.set(http::field::content_type, "application/json");
    response.body() = json_response.dump();
    response.prepare_payload();

    http::message_generator generator(std::move(response));
    SendResponse(std::move(generator));
}

void Session::GetUserCallback(ResultType<User>& result) {

    if (!result.IsSuccess()) {
        BadRequestCallback(result.GetError()->what());
        return;
    }

    auto user = result.GetResult();
    http::response<http::string_body> response;

    response.version(11);
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    nlohmann::json json_response(user);
    response.result(http::status::ok);
    response.set(http::field::content_type, "application/json");
    response.body() = json_response.dump();
    response.prepare_payload();

    http::message_generator generator(std::move(response));
    SendResponse(std::move(generator));
}

void Session::GetVoidCallback(ResultType<void>& result) {

    if (!result.IsSuccess()) {
        BadRequestCallback(result.GetError()->what());
        return;
    }

    http::response<http::string_body> response;

    response.version(11);
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.result(http::status::ok);
    response.set(http::field::content_type, "application/json");
    response.prepare_payload();

    http::message_generator generator(std::move(response));
    SendResponse(std::move(generator));
}       

void Session::BadRequestCallback(const std::string& message){
    http::response<http::string_body> response;
    response.version(11);
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.result(http::status::bad_request);
    response.set(http::field::content_type, "text/html");
    response.body() = message;
    response.prepare_payload();
    http::message_generator generator(std::move(response));
    SendResponse(std::move(generator));
}

void Session::HandleRequest(
    beast::string_view doc_root
    , http::request<http::string_body>&& req
    , std::shared_ptr<Server>& server)
{
    http::response<http::string_body> response;

    if (req.target() == "/api/list_all_stocks" && req.method() == http::verb::get) {

        std::function<void(ResultType<std::vector<Stock>>&)> callback = [session = shared_from_this()](ResultType<std::vector<Stock>>& stocks) {
            session->GetStocksCallback(stocks);
        };
        server->ListStocks(callback);

    } else if (req.target() == "/api/buy_stocks" && req.method() == http::verb::post){
        
        auto json_request = nlohmann::json::parse(req.body());

        if (!json_request.contains("user_id") || !json_request.contains("stock_id") || !json_request.contains("count")){
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("");
            };
            server->BadRequest(callback);
        }
        uint64_t user_id = json_request["user_id"];
        UserStock user_stock(json_request["stock_id"],  json_request["count"]);
        
        std::function<void(ResultType<void>&)> callback = [session = shared_from_this()](ResultType<void>& result) {
            session->GetVoidCallback(result);
        };

        server->BuyStocks(user_id, user_stock, callback);

    } else if (req.target() == "/api/sell_stocks" && req.method() == http::verb::post){
        
        auto json_request = nlohmann::json::parse(req.body());

        if (!json_request.contains("user_id") || !json_request.contains("stock_id") || !json_request.contains("count")){
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("");
            };
            server->BadRequest(callback);
        }
        
        uint64_t user_id = json_request["user_id"];
        UserStock user_stock(json_request["stock_id"],  json_request["count"]);
        
        std::function<void(ResultType<void>&)> callback = [session = shared_from_this()](ResultType<void>& result) {
            session->GetVoidCallback(result);
        };

        server->SellStocks(user_id, user_stock, callback);

    } else if (req.target() == "/api/register" && req.method() == http::verb::post){
        
        auto json_request = nlohmann::json::parse(req.body());

        if (!json_request.contains("name") || !json_request.contains("email") || !json_request.contains("password")){
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("");
            };
            server->BadRequest(callback);
        }

        std::string name = json_request["name"];
        std::string email = json_request["email"];
        std::string password = json_request["password"];
        User user(0, name, email, password, 0);
        
        std::function<void(ResultType<uint64_t>&)> callback = [session = shared_from_this()](ResultType<uint64_t>& result) {
            session->GetIdCallback(result);
        };

        server->Register(user, callback);

    } else if (req.target() == "/api/login_user" && req.method() == http::verb::post){
        
        auto json = nlohmann::json::parse(req.body());

        if (!json.contains("email") || !json.contains("password")){
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("");
            };
            server->BadRequest(callback);
        }

        std::string email = json["email"];
        std::string password = json["password"];
        
        std::function<void(ResultType<User>&)> callback = [session = shared_from_this()](ResultType<User>& result) {
            session->GetUserCallback(result);
        };

        server->Login(email, password, callback);
    }
    else if (req.target() == "/api/change_password" && req.method() == http::verb::post){
        
        auto json_request = nlohmann::json::parse(req.body());

        if (!json_request.contains("user_id") || !json_request.contains("password")){
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("");
            };
            server->BadRequest(callback);
        }

        uint64_t user_id = json_request["user_id"];
        std::string password = json_request["password"];
        
        std::function<void(ResultType<void>&)> callback = [session = shared_from_this()](ResultType<void>& result) {
            session->GetVoidCallback(result);
        };

        server->ChangePassword(user_id, password, callback);

    } else if (req.target() == "/api/list_user_stock" && req.method() == http::verb::get){
        
        auto json_request = nlohmann::json::parse(req.body());
        
        if (!json_request.contains("user_id")) {
            std::function<void()> callback = [session = shared_from_this()]() {
                session->BadRequestCallback("user must be authorized");
            };
            server->BadRequest(callback);
            return;
        } 
            
        uint64_t user_id = json_request["user_id"];
        std::function<void(ResultType<std::vector<Stock>>)> callback = [session = shared_from_this()](ResultType<std::vector<Stock>> result) {
            session->GetStocksCallback(result);
        };

        server->ListUsersStock(user_id, callback);

    } else {
        response.result(http::status::not_found);
        response.prepare_payload();
    }

}

void to_json(nlohmann::json &j, const Stock &stock){
    j = nlohmann::json{
        {"id", stock.id_},
        {"cost", stock.cost_},
        {"count", stock.count_},
        {"company_name", stock.company_name_},
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
        {"email", user.email_},
        {"password", user.password_},
        {"balance", user.balance_},
    };
}

void from_json(nlohmann::json &j, User &user){
    j.at("id").get_to(user.id_);
    j.at("name").get_to(user.name_);
    j.at("email").get_to(user.email_);
    j.at("password").get_to(user.password_);
    j.at("balance").get_to(user.balance_);
}