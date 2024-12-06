set(LIST_HEADERS
        List.h
)

add_library(List INTERFACE)
target_include_directories(List INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
