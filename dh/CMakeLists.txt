find_package(PkgConfig)
pkg_check_modules(BOTAN3 botan-3 REQUIRED)

add_library(
    dh STATIC
    dh.cpp
)

target_link_libraries(
    dh
    PRIVATE
    botan-3
    ${BOTAN3_LDFLAGS}
)

target_include_directories(
    dh
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${BOTAN3_INCLUDE_DIRS}
)
