cmake_minimum_required(VERSION 3.31)
project(netlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Boost COMPONENTS asio beast url unit_test_framework REQUIRED)

if (MSVC)
    add_compile_options(/bigobj)
endif()

add_executable(netlib_test tests/http_test.cpp)

target_link_libraries(netlib_test netlib Boost::asio Boost::beast Boost::url)


add_test(NAME netlib_test COMMAND netlib_test)
