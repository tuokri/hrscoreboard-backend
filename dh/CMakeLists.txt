add_library(
    dh STATIC
    dh.cpp
)

target_include_directories(
    dh PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
