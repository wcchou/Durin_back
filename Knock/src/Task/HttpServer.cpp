#include <Config/Config.hpp>

#include <Task/HttpServer.hpp>

#include <vector>
#include <iostream>

namespace {

void fail(boost::beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

} // unnamed namespace

namespace Knock {

boost::beast::string_view mime_type( boost::beast::string_view path )
{
    using boost::beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == boost::beast::string_view::npos)
            return boost::beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(
    boost::beast::string_view base,
    boost::beast::string_view path)
{
    if(base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

template <typename Body, typename Allocator, typename Send>
void handleRequest(
    boost::beast::string_view doc_root,
    boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
    Send&& send )
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::bad_request, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](boost::beast::string_view target)
    {
        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::not_found, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](boost::beast::string_view what)
    {
        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::internal_server_error, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if( req.method() != boost::beast::http::verb::get &&
        req.method() != boost::beast::http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
        path.append("index.html");

    // Attempt to open the file
    boost::beast::error_code ec;
    boost::beast::http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if(ec == boost::beast::errc::no_such_file_or_directory)
        return send(not_found(req.target()));

    // Handle an unknown error
    if(ec)
        return send(server_error(ec.message()));

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == boost::beast::http::verb::head)
    {
        boost::beast::http::response<boost::beast::http::empty_body> res{boost::beast::http::status::ok, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }

    // Respond to GET request
    boost::beast::http::response<boost::beast::http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(boost::beast::http::status::ok, req.version())};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, mime_type(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
}

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    class Queue {
        enum {
            limit = 8
        };

        struct Work {
            virtual ~Work() = default;

            virtual void operator ()() = 0;
        };

        HttpSession& self_;
        std::vector<std::unique_ptr<Work>> items_;

    public:
        explicit
        Queue( HttpSession& self )
            : self_( self )
        {
            items_.reserve( limit );
        }

        bool isFull() const { return items_.size() >= limit; }

        bool onWrite()
        {
            BOOST_ASSERT( !items_.empty() );

            const auto wasFull = isFull();
            items_.erase( items_.begin() );
            if ( !items_.empty() ) {
                (*items_.front())();
            }
            return wasFull;
        }

        template <bool isRequest, typename Body, typename Fields>
        void operator ()( boost::beast::http::message<isRequest, Body, Fields>&& msg )
        {
            struct WorkImpl : public Work {
                HttpSession& self_;
                boost::beast::http::message<isRequest, Body, Fields> msg_;

                WorkImpl( HttpSession& self, boost::beast::http::message<isRequest, Body, Fields>&& msg )
                    : self_( self ),
                      msg_( std::move( msg ) )
                {
                }

                void operator ()()
                {
                    boost::beast::http::async_write(
                        self_.stream_,
                        msg_,
                        boost::beast::bind_front_handler(
                            &HttpSession::onWrite,
                            self_.shared_from_this(),
                            msg_.need_eof()
                        )
                    );
                }
            };

            items_.push_back(
                boost::make_unique<WorkImpl>( self_, std::move( msg ) )
            );

            if ( items_.size() == 1 ) {
                (*items_.front())();
            }
        }
    };

    boost::beast::tcp_stream    stream_;
    boost::beast::flat_buffer   buffer_;
    std::shared_ptr<const std::string> docRoot_;
    Queue                              queue_;
    boost::optional<boost::beast::http::request_parser<boost::beast::http::string_body>>    parser_;

public:
    HttpSession( boost::asio::ip::tcp::socket&& socket, std::shared_ptr<const std::string> docRoot )
        : stream_{ std::move( socket ) },
            docRoot_( docRoot ),
            queue_( *this )
    {
    }

    void run()
    {
        doRead();
    }

private:
    void doRead()
    {
        parser_.emplace();
        parser_->body_limit( 10000 );

        stream_.expires_after( std::chrono::seconds( 30 ) );

        boost::beast::http::async_read(
            stream_,
            buffer_,
            *parser_,
            boost::beast::bind_front_handler(
                &HttpSession::onRead,
                shared_from_this()
            )
        );
    }

    void onRead( boost::beast::error_code ec, std::size_t readBytes )
    {
        boost::ignore_unused( readBytes );

        if ( boost::beast::http::error::end_of_stream == ec ) {
            return doClose();
        }

        if ( ec ) {
            return fail( ec, "read" );
        }

        handleRequest( *docRoot_, parser_->release(), queue_ );

        if ( !queue_.isFull() ) {
            doRead();
        }
    }

    void onWrite( bool close, boost::beast::error_code ec, std::size_t writtenBytes )
    {
        boost::ignore_unused( writtenBytes );

        if ( ec ) {
            return fail( ec, "write" );
        }

        if ( close ) {
            return doClose();
        }

        if ( queue_.onWrite() ) {
            doRead();
        }
    }

    void doClose()
    {
        boost::beast::error_code ec;
        stream_.socket().shutdown( boost::asio::ip::tcp::socket::shutdown_send, ec );
    }
};

HttpListener::HttpListener( boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint,
    std::shared_ptr<const std::string> docRoot)
    : ioc_( ioc ),
      acceptor_( ioc_ ),
      docRoot_( docRoot )
{
    boost::beast::error_code ec;

    acceptor_.open( endpoint.protocol(), ec );
    if ( ec ) {
        fail( ec, "open" );
        return;
    }

    acceptor_.set_option(
        boost::asio::socket_base::reuse_address( true ), ec );
    if ( ec ) {
        fail( ec, "set_option" );
        return;
    }

    acceptor_.bind( endpoint, ec );
    if ( ec ) {
        fail( ec, "bind" );
        return;
    }

    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec );
    if ( ec ) {
        fail( ec, "listen" );
        return;
    }
}

void HttpListener::run()
{
    doAccept();
}

void HttpListener::doAccept()
{
    acceptor_.async_accept(
        ioc_,
        boost::beast::bind_front_handler(
            &HttpListener::onAccept,
            shared_from_this()
        )
    );
}

void HttpListener::onAccept( boost::beast::error_code ec, boost::asio::ip::tcp::socket socket )
{
    if ( ec ) {
        fail( ec, "accept" );
        return;
    }

    std::make_shared<HttpSession>(
        std::move( socket ),
        docRoot_
    )->run();

    doAccept();
}

bool HttpServer::init( const std::string& address, const std::string& port,
    const std::string& docRoot )
{
    address_ = boost::asio::ip::make_address( address );
    port_    = static_cast<unsigned short>( std::atoi( port.c_str() ) );

    listener_ = std::make_shared<HttpListener>(
        ioc_,
        boost::asio::ip::tcp::endpoint{ address_, port_ },
        std::make_shared<const std::string>( docRoot ) );
    return true;
}

bool HttpServer::start()
{
    listener_->run();

    threads_.emplace_back(
        [this] {
            ioc_.run();
        }
    );

    ioc_.run();

    return true;
}

bool HttpServer::wait()
{
    for ( auto& thread : threads_ ) {
        thread.join();
    }

    return true;
}

} // namespace Knock