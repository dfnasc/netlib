//
// Created by diego.nascimento on 30/12/2025.
//

#ifndef NETLIB_HTTP_SERVER_HPP
#define NETLIB_HTTP_SERVER_HPP

#include "connection.hpp"

namespace net {

    namespace http {

        /*
         * http::server class implementation
         */

        class server : public std::enable_shared_from_this<server> {
        public:

            friend std::shared_ptr<server> make_server(
                        ip::port_type port = 8080,
                        io_context& ioc = global::instance().ioc
                    );

            friend std::shared_ptr<server> make_server(
                        std::string_view address = "0.0.0.0",
                        std::uint32_t port = 8080,
                        io_context& ioc = global::instance().ioc
                    );

            void get(std::string_view path, request_handler handler) { router_.get(path, handler); };
            void post(std::string_view path, request_handler handler) { router_.post(path, handler); };
            void put(std::string_view path, request_handler handler) { router_.put(path, handler); };
            void patch(std::string_view path, request_handler handler) { router_.patch(path, handler); };
            void del(std::string_view path, request_handler handler) { router_.del(path, handler); };
            void head(std::string_view path, request_handler handler) { router_.head(path, handler); };
            void options(std::string_view path, request_handler handler) { router_.options(path, handler); };
            void trace(std::string_view path, request_handler handler) { router_.trace(path, handler); };

            void start() {
                do_accept();
            }

        private:

            explicit server(ip::port_type port, io_context& ioc)
                : ioc_{ioc},
                port_{8080},
                acceptor_{ioc, tcp::endpoint{ip::address_v4::any(), port_}} {
                }

            explicit server(std::string_view address, ip::port_type port, boost::asio::io_context& ioc)
                : ioc_{ioc},
                port_{port},
                acceptor_{ioc, tcp::endpoint{ip::make_address(address), port_}} {
            }

            void do_accept() {
                acceptor_.async_accept([s=this->shared_from_this()](const error_code& ec, tcp::socket&& sock) {

                    if (ec) {
                        // TODO: log error
                    } else {
                        std::cout << "new connection: " << sock.remote_endpoint().address().to_string() << ":" << sock.remote_endpoint().port() << std::endl;
                        make_connection(s->ioc_, std::move(sock), s->router_)->start();
                    }

                    s->do_accept();
                });
            }

            net::io_context& ioc_;
            ip::port_type port_;
            tcp::acceptor acceptor_;
            request_router router_;
        }; // end of class server

        inline std::shared_ptr<server> make_server(ip::port_type port, io_context& ioc) {
            return std::shared_ptr<server>{new server(port, ioc)};
        }

        inline std::shared_ptr<server> make_server(std::string_view address, std::uint32_t port, io_context& ioc) {
            return std::shared_ptr<server>{new server(address, port, ioc)};
        }

    } // end of namespace http
} // end of namespace net

#endif // NETLIB_HTTP_SERVER_HPP
