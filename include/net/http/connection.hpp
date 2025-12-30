#ifndef NETLIB_HTTP_CONNECTION_H
#define NETLIB_HTTP_CONNECTION_H

#include "router.hpp"

namespace net::http {

        /*
         * http::connection implementation
         */

        class connection : public std::enable_shared_from_this<connection> {
        public:

            friend std::shared_ptr<connection> make_connection(io_context& ioc, tcp_socket&& socket, request_router& router);

            void start() {
                do_read_header();
            }

        private:
            connection(io_context& ioc, tcp_socket&& socket, request_router& router)
                : ioc_(ioc),
                router_{router},
                stream_{std::move(socket)} {

                parser_.header_limit(1024*64); // 64Kb
                parser_.body_limit(1024*1024*100); // 100Mb
            }

            void do_read_header() {
                boost::beast::http::async_read_header(stream_, buffer_, parser_, [conn=this->shared_from_this()](const error_code& ec, size_t bytes_transfered) {
                    boost::ignore_unused(bytes_transfered);

                    if (ec) {
                        std::cout << "Oops: " << ec.message() << std::endl;
                        conn->do_close();
                    } else {
                        auto& req = conn->parser_.get();
                        auto verb = req.method();

                        conn->handler_ = conn->router_.find(req);

                        if (verb == method::post || verb == method::put || verb == method::patch) {
                            conn->do_read_body();
                        } else {
                            conn->do_handle();
                        }
                    }
                });
            }

            void do_read_body() {
                boost::beast::http::async_read(stream_, buffer_, parser_, [conn=this->shared_from_this()](const error_code& ec, size_t bytes_transfered) {

                    conn->response_.version(11);
                    conn->response_.set(field::user_agent, "netlib");
                    conn->do_handle();
                });
            }

            void do_handle() {
                handler_(request_context{stream_.socket(), parser_.get(), response_});
                response_.prepare_payload();
                do_respond();
            }

            void do_respond() {
                boost::beast::http::async_write(stream_, response_, [conn=this->shared_from_this()](const error_code& ec, size_t bytes_transfered) {

                    if (ec) {
                        // TODO: log error
                    }

                    conn->do_close();
                });
            }

            void do_close() {
                stream_.socket().shutdown(tcp::socket::shutdown_both);
            }

            io_context& ioc_;
            request_router& router_;
            tcp_stream stream_;
            request_parser parser_;
            flat_buffer buffer_;
            response response_;
            request_handler handler_;
        };

        inline std::shared_ptr<connection> make_connection(io_context& ioc, tcp_socket&& socket, request_router& router) {
            return std::shared_ptr<connection>{new connection(ioc, std::forward<tcp_socket>(socket), router)};
        }
} // end of namespace net::http

#endif // NNETLIB_HTTP_CONNECTION_H