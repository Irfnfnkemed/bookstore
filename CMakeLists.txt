cmake_minimum_required(VERSION 3.22)
project(bookstroreOfWGJ)

set(CMAKE_CXX_STANDARD 14)

add_executable(bookstroreOfWGJ main.cpp blockchain.cpp blockchain.h login.cpp login.h account.cpp account.h error.cpp error.h book.cpp book.h translate.cpp translate.h)
