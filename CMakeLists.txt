cmake_minimum_required(VERSION 3.10)
project(QtStreamDeck VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Svg)
find_package(hidapi REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)
