set(VECTOR_HEADERS
    Vector.h
)

add_library(Vector INTERFACE)
target_include_directories(Vector INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
