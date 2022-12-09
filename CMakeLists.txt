cmake_minimum_required(VERSION 3.23)
project(hrscoreboard_backend)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Werror -Wpedantic)

find_package(
    Boost 1.80
    COMPONENTS
    system
    REQUIRED
)

add_library(
    xxtea STATIC
    xxtea.cpp
)

add_executable(
    hrscoreboard_packethandler
    main.cpp
)

target_link_libraries(
    hrscoreboard_packethandler
    PRIVATE
    xxtea
    pthread
    ${Boost_LIBRARIES}
)
