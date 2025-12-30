#ifndef NETLIB_NET_HPP
#define NETLIB_NET_HPP

#include "aliases.hpp"

namespace net {

    struct global {

        io_context ioc;

        static global& instance() {
            static global instance_;
            return instance_;
        }
    };
}

#endif // NETLIB_NET_HPP