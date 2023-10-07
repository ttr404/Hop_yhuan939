cmake_minimum_required(VERSION 3.26)
project(Crow)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Crow
        include/crow_all.h
        main.cpp)


list(APPEND CMAKE_PREFIX_PATH "/opt")
target_link_libraries(Crow PRIVATE pthread)
include_directories(/usr/local/include)
# include_directories(/opt/homebrew/Cellar/asio/1.28.1/include)


file(COPY ${CMAKE_SOURCE_DIR}/public DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/static DESTINATION ${CMAKE_BINARY_DIR})
