cmake_minimum_required(VERSION 3.16)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tftp-server VERSION 0.1 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Quick)

add_subdirectory(app)
