#include "Session.h"

Session::Session(asio::ip::tcp::socket socket, std::shared_ptr<const std::string>& doc_root): 
    stream_(std::move(socket))
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

    SendResponse(HandleRequest(*doc_root_, std::move(req_)));
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

http::message_generator HandleRequest(beast::string_view doc_root, http::request<http::string_body>&& req)
{
    // std::string path = doc_root + req.target();
    http::response<http::string_body> response;

    if (req.method() == http::verb::get) {
        response.result(http::status::ok);
        response.version(11);
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.body() = "Hello world\n";
        response.prepare_payload();
    }

    return response;
}
