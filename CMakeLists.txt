cmake_minimum_required(VERSION 3.26)
project(Crow)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Crow
        include/crow_all.h
        main.cpp)

# Find the CURL library
find_package(CURL REQUIRED)

# If CURL is found, add the include directories to your project
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(Crow PRIVATE ${CURL_LIBRARIES})
else()
    message(FATAL_ERROR "CURL library not found")
endif()

# Find Asio library
list(APPEND CMAKE_PREFIX_PATH "/opt")
target_link_libraries(Crow PRIVATE pthread)
include_directories(/usr/local/include)
include_directories(/opt/homebrew/Cellar/asio/1.28.1/include)

# Copy public and static folders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/public DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/static DESTINATION ${CMAKE_BINARY_DIR})
