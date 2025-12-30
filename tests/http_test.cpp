//
// Created by diego.nascimento on 28/07/2025.
//

#include "../include/net/http.hpp"

#define BOOST_TEST_MODULE HTTP Tests
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(HTTPTestCase) {

    net::io_context ioc;

    auto server = net::http::make_server(8080, ioc);

    server->get("/", [&](const net::http::request_context& rc) {
        rc.response.result(net::http::status::ok);
        rc.response.body() = "Hello world!";
    });

    server->post("/", [&](const net::http::request_context& rc) {

        std::cout << "Post Body: \n" << rc.request.body() << "\n\n" << std::flush;

        rc.response.result(net::http::status::ok);
        rc.response.body() = "Post Message received!";
    });

    server->put("/", [&](const net::http::request_context& rc) {

        std::cout << "Put Body: \n" << rc.request.body() << "\n\n" << std::flush;

        rc.response.result(net::http::status::ok);
        rc.response.body() = "Put Message received!";
    });

    server->patch("/", [&](const net::http::request_context& rc) {

        std::cout << "Patch Body: \n" << rc.request.body() << "\n\n" << std::flush;

        rc.response.result(net::http::status::ok);
        rc.response.body() = "Patch Message received!";
    });

    server->get("/quit", [&](const net::http::request_context& rc) {
        rc.response.result(net::http::status::ok);
        rc.response.body() = "Goodbye!";
        ioc.stop();
    });

    server->start();

    ioc.run();

    BOOST_CHECK_EQUAL(1, 1);
}