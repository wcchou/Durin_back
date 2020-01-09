#ifndef KNOCK_TASK_HTTP_SERVER_HPP
#define KNOCK_TASK_HTTP_SERVER_HPP

#include <Config/Config.hpp>

#include <string>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <Durin/Type/BasicTypes.hpp>

namespace Knock {

class HttpListener : public std::enable_shared_from_this<HttpListener> {
public:
    HttpListener( boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint,
        std::shared_ptr<const std::string> docRoot );

    void run();

private:
    void doAccept();
    void onAccept( boost::beast::error_code ec, boost::asio::ip::tcp::socket socket );

    boost::asio::io_context&            ioc_;
    boost::asio::ip::tcp::acceptor      acceptor_;
    std::shared_ptr<const std::string>  docRoot_;
};

class HttpServer {
public:
    bool init( const std::string& address, const std::string& port, const std::string& docRoot );
    bool start();
    bool wait();

    const boost::asio::ip::address& address() const { return address_; }
    Durin::kuint16 port() const { return port_; }

private:
    boost::asio::io_context         ioc_;
    boost::asio::ip::address        address_;
    Durin::kuint16                  port_;
    std::shared_ptr<HttpListener>   listener_;
    std::vector<std::thread>        threads_;
};

} // namespace Knock

#endif // End of include guard
