
cmake_minimum_required(VERSION 3.20)

project(ExchangeOrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# Backend library
add_library(exchange_core
    src/Order.cpp
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/Trade.cpp
)

target_include_directories(exchange_core PUBLIC include)

# Console application
add_executable(exchange
    src/main.cpp
)

target_link_libraries(exchange PRIVATE exchange_core)

# Qt GUI application
qt_add_executable(exchange_gui
    src/gui/main.cpp
    src/MainWindow.cpp
    include/MainWindow.hpp
)

target_link_libraries(exchange_gui PRIVATE
    exchange_core
    Qt6::Widgets
)
