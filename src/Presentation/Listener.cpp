#include "Listener.h"

Listener::Listener
(
    asio::io_context &context
    , ip::tcp::endpoint endpoint
    , std::shared_ptr<Server>& server
    , std::shared_ptr<const std::string> &doc_root
) : 
    context_(context)
    , acceptor_(asio::make_strand(context))
    , server_(server)
    , doc_root_(doc_root)
{

    beast::error_code ec;
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor_.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor_.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }
}

void Listener::Run(){
    DoAccept();
}

void Listener::DoAccept(){
    acceptor_.async_accept(asio::make_strand(context_), beast::bind_front_handler(
        &Listener::OnAccept, shared_from_this()
    ));
}

void Listener::OnAccept(beast::error_code ec, ip::tcp::socket socket){
    if (ec) {
        std::cerr << ec.message() << "\n";
    } else {
        std::make_shared<Session>(std::move(socket), server_, doc_root_)->Run();
    }

    DoAccept();
}
