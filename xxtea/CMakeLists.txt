add_library(
    xxtea STATIC
    xxtea.cpp
)

target_include_directories(
    xxtea PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET xxtea PROPERTY ENABLE_EXPORTS $<$<CONFIG:Debug>:ON,OFF>)
