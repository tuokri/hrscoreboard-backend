add_library(
    xxtea STATIC
    xxtea.cpp
)

target_include_directories(
    xxtea PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
