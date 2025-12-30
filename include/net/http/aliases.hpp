#ifndef ALIASES_H
#define ALIASES_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace net {
    namespace ip = boost::asio::ip;
    using tcp = ip::tcp;
    using udp = ip::udp;

    using error_code = boost::system::error_code;
    using io_context = boost::asio::io_context;
    using tcp_socket = tcp::socket;
    using udp_socket = udp::socket;
    using flat_buffer = boost::beast::flat_buffer;
    namespace urls = boost::urls;

    namespace http {
        using tcp_stream = boost::beast::tcp_stream;
        using response = boost::beast::http::response<boost::beast::http::string_body>;
        using request = boost::beast::http::request<boost::beast::http::string_body>;
        using request_parser = boost::beast::http::request_parser<boost::beast::http::string_body>;
        using method = boost::beast::http::verb;
        using field = boost::beast::http::field;
        using status = boost::beast::http::status;
    }
}

#endif