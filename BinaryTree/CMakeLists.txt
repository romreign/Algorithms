set(BINARYTREE_HEADERS
        binarytree.h
)

add_library(BinaryTree INTERFACE)
target_include_directories(BinaryTree INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
