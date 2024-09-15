#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"
#include "BinaryTree.h"
#include <iostream>

TEST_CASE("FIND & GET POINTER")
{
    BinaryTree<int>* root{};
    root->inserter(9);
    root->inserter(6);
    root->inserter(8);
    root->inserter(17);
    root->inserter(15);
    root->inserter(20);

    SECTION("search function")
    {
        SECTION("not found")
            REQUIRE(root->find(11) == false);
        SECTION("found")
            REQUIRE(root->find(8) == true);
    }
    SECTION("the function of getting pointer")
    {
        SECTION("pointer was not found")
            REQUIRE(root->get_pointer(11) == nullptr);
        SECTION("pointer was found")
            REQUIRE(root->get_pointer(8) != nullptr);
    }
}

TEST_CASE("EMPTY & GET SIZE")
{
    BinaryTree<int>* root{};

    SECTION("initial tree size 0 & empty")
    {
        REQUIRE(root->is_empty() == true);
        REQUIRE(root->get_size() == 0);
    }
    SECTION("the size after adding")
    {
        root->inserter(9);
        root->inserter(6);
        root->inserter(8);
        root->inserter(17);
        root->inserter(15);
        root->inserter(20);

        REQUIRE(root->is_empty() == false);
        REQUIRE(root->get_size() == 6);
    }
    SECTION("the size after deleting")
    {
        root->inserter(9);
        root->inserter(6);
        root->inserter(8);
        root->inserter(17);
        root->inserter(15);
        root->inserter(20);

        root->delete_val(8);
        root->delete_val(17);

        REQUIRE(root->is_empty() == false);
        REQUIRE(root->get_size() == 4);
    }
}

TEST_CASE("INORDER TRAVERSAL & INSERT")
{
    BinaryTree<int>* root{};
    std::vector<int> check_correct;

    auto correct = std::vector<int> {3, 6, 7, 8, 9, 12, 15, 16, 17, 19, 20};

    root->inserter(9);

    root->inserter(6);
    root->inserter(3);
    root->inserter(8);
    root->inserter(7);

    root->inserter(17);
    root->inserter(15);
    root->inserter(20);
    root->inserter(12);
    root->inserter(16);
    root->inserter(19);

    REQUIRE(root->in_order_traversal() == correct);
}

TEST_CASE("PREORDER TRAVERSAL")
{
    BinaryTree<int>* root{};
    auto correct = std::vector<int> {9, 6, 3, 8, 7, 17, 15, 12, 16, 20, 19};

    root->inserter(9);

    root->inserter(6);
    root->inserter(3);
    root->inserter(8);
    root->inserter(7);

    root->inserter(17);
    root->inserter(15);
    root->inserter(20);
    root->inserter(12);
    root->inserter(16);
    root->inserter(19);

    REQUIRE(root->pre_order_traversal() == correct);
}

TEST_CASE("POSTORDER TRAVERSAL")
{
    BinaryTree<int>* root{};
    auto correct = std::vector<int> {3, 7, 8, 6, 12, 16, 15, 19, 20, 17, 9};

    root->inserter(9);

    root->inserter(6);
    root->inserter(3);
    root->inserter(8);
    root->inserter(7);

    root->inserter(17);
    root->inserter(15);
    root->inserter(20);
    root->inserter(12);
    root->inserter(16);
    root->inserter(19);

    REQUIRE(root->post_order_traversal() == correct);
}

TEST_CASE("DELETE NODE")
{
    BinaryTree<int>* root{};
    root->inserter(9);
    root->inserter(6);
    root->inserter(8);
    root->inserter(17);
    root->inserter(15);
    root->inserter(20);

    SECTION("found")
        REQUIRE(root->find(17) == true);

    SECTION("not found")
    {
            root->delete_val(17);
        REQUIRE(root->find(17) == false);
    }
}

TEST_CASE("IS BALANCED")
{
    BinaryTree<int>* root{};

    SECTION("not balanced")
    {
        root->inserter(9);
        root->inserter(17);
        root->inserter(15);
        root->inserter(20);
        root->inserter(19);
        root->inserter(22);
        root->inserter(24);
        REQUIRE(root->is_balanced() == false);
    }

    SECTION("balanced")
    {
        root->inserter(9);
        root->inserter(6);
        root->inserter(17);
        root->inserter(3);
        root->inserter(8);
        root->inserter(20);
        REQUIRE(root->is_balanced() == true);
    }
}

TEST_CASE("MIN & MAX POINTER")
{
    BinaryTree<int>* root{};
    root->inserter(9);
    root->inserter(6);
    root->inserter(8);
    root->inserter(17);
    root->inserter(15);
    root->inserter(20);
    SECTION("min")
        REQUIRE(root->get_pointer_min() == root->get_pointer(6));
    SECTION("max")
        REQUIRE(root->get_pointer_max() == root->get_pointer(20));
}

TEST_CASE("HEIGHT BINARY TREE")
{
    BinaryTree<int>* root{};
    root->inserter(9);
    root->inserter(5);
    root->inserter(17);
    root->inserter(6);
    root->inserter(15);
    root->inserter(20);
    root->inserter(7);
    root->inserter(23);
    REQUIRE(root->height() == 4);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
}

