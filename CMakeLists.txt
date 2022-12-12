cmake_minimum_required(VERSION 3.23)
project(hrscoreboard_backend)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Werror -pedantic -Wpedantic -Wextra -fcoroutines)

set(Boost_USE_DEBUG_LIBS $<$<CONFIG:Debug>:ON,OFF>)

add_compile_definitions(
    $<$<CONFIG:Debug>:BOOST_STACKTRACE_LINK>
    $<$<CONFIG:Debug>:BOOST_STACKTRACE_DYN_LINK>
    $<$<CONFIG:Debug>:BOOST_STACKTRACE_USE_ADDR2LINE>
    $<$<CONFIG:Release>:BOOST_STACKTRACE_USE_NOOP>
)

find_package(
    Boost 1.80
    COMPONENTS
    system
    coroutine
    stacktrace_addr2line
    stacktrace_noop
    REQUIRED
)

add_subdirectory(xxtea)
add_subdirectory(dh)

add_executable(
    hrscoreboard_packethandler
    main.cpp
)

target_link_libraries(
    hrscoreboard_packethandler PRIVATE
    xxtea
    Boost::system
    Boost::coroutine
    debug Boost::stacktrace_addr2line
    optimized Boost::stacktrace_noop
    debug dl
    debug backtrace
    pthread
)

set_property(TARGET hrscoreboard_packethandler PROPERTY ENABLE_EXPORTS $<$<CONFIG:Debug>:ON,OFF>)

target_link_options(
    hrscoreboard_packethandler PRIVATE
    $<$<CONFIG:Debug>:-no-pie>
    $<$<CONFIG:Debug>:-fno-pie>
)

target_compile_options(
    hrscoreboard_packethandler PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Debug>:-ggdb>
    $<$<CONFIG:Debug>:-no-pie>
    $<$<CONFIG:Debug>:-fno-pie>
)
