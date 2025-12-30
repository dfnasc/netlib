#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "net.hpp"

namespace net::http {

    /*
         * http::request_context
         */

        struct request_context {
            tcp_socket& socket;
            http::request& request;
            http::response& response;
        };

        using request_handler = std::function<void (const request_context& ctx)>;

        /*
         * http::router implementation
         */

        class request_router {
        public:
            request_router() = default;

            request_router& get(std::string_view path, request_handler handler) { return map(method::get, path, handler); }
            request_router& post(std::string_view path, request_handler handler) { return map(method::post, path, handler); }
            request_router& put(std::string_view path, request_handler handler) { return map(method::put, path, handler); }
            request_router& patch(std::string_view path, request_handler handler) { return map(method::patch, path, handler); }
            request_router& del(std::string_view path, request_handler handler) { return map(method::delete_, path, handler); }
            request_router& head(std::string_view path, request_handler handler) { return map(method::head, path, handler); }
            request_router& trace(std::string_view path, request_handler handler) { return map(method::trace, path, handler); }
            request_router& options(std::string_view path, request_handler handler) { return map(method::options, path, handler); }

            request_router& map(method verb, std::string_view path, request_handler handler) {
                rhandlers_[hkey(verb, path)] = handler;
                return *this;
            }

            request_handler find(request req) {
                std::stringstream surl;
                surl << "http://x.y" << req.target();

                return find(req.method(), urls::url_view{surl.str()}.path());
            }

            request_handler find(method verb, std::string_view path) {
                auto it = rhandlers_.find(hkey(verb, path));

                if (it != rhandlers_.end()) {
                    return it->second;
                }

                return default_request_handler;
            }

            static void default_request_handler(const request_context& rc) {
                rc.response.result(status::not_found);
                rc.response.set("Content-Type", "text/html");
                rc.response.body() = "400 not found";
            }

        private:

            static std::string hkey(method verb, std::string_view path) {

                std::stringstream ss;

                ss << boost::beast::http::to_string(method::get)
                   << ":"
                   << path;

                return ss.str();
            }

            std::unordered_map<std::string, request_handler> rhandlers_;
        };

} // end of namespace net::http

#endif // HTTP_ROUTER_H