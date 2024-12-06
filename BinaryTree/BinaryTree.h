#ifndef BINARYTREE_BINARYTREE_H
#define BINARYTREE_BINARYTREE_H

#include <memory>
#include <vector>
#include <stack>
#include <functional>
#include <queue>

template <typename T, typename Allocator = std::allocator<T>>
class BinaryTree {
    using Type = T;

    static inline const auto m_end = nullptr;

    struct TreeNode {
        friend class BinaryTree;

        TreeNode();
        TreeNode(Type in_val, TreeNode* in_left, TreeNode* in_right);
        explicit TreeNode(Type in_val);

    private:
        Type val;
        TreeNode* left;
        TreeNode* right;
    };



    Allocator alloc;
    TreeNode* m_root;
    size_t m_size;

    void deleter() noexcept;

public:
    explicit BinaryTree(TreeNode* in_root);
    BinaryTree();
    BinaryTree(BinaryTree &other) = delete;
    BinaryTree(BinaryTree &&other) noexcept;

    ~BinaryTree();

    BinaryTree &operator=(const BinaryTree &other) = delete;
    BinaryTree &operator=(BinaryTree &&other) noexcept;

    std::vector<Type> pre_order_traversal() const noexcept;
    std::vector<Type> in_order_traversal() const noexcept;
    std::vector<Type> post_order_traversal() const noexcept;
    std::vector<Type> level_order_traversal() const noexcept;

    [[nodiscard]] bool is_balanced() const noexcept;
    [[nodiscard]] bool is_empty() const noexcept;
    [[nodiscard]] bool find(Type in_val) const noexcept;

    [[nodiscard]] size_t get_size() const noexcept;

    [[nodiscard]] int height() const noexcept;

    void inserter(Type in_val) noexcept;
    void delete_val(Type in_val) noexcept;

    TreeNode *get_pointer(Type in_val) const noexcept;
    TreeNode *get_pointer_min() const noexcept;
    TreeNode *get_pointer_max() const noexcept;

};

//TreeNode
template <typename T, typename Allocator>
BinaryTree<T, Allocator>::TreeNode::TreeNode() : val{}, left(m_end), right(m_end) {}

template <typename T, typename Allocator>
BinaryTree<T, Allocator>::TreeNode::TreeNode(T in_val, TreeNode* in_left, TreeNode* in_right)
        : val(in_val), left(in_left), right(in_right) {}

template <typename T, typename Allocator>
BinaryTree<T, Allocator>::TreeNode::TreeNode(T in_val) :
        BinaryTree<T, Allocator>::TreeNode::TreeNode(in_val, m_end, m_end) {}



//BinaryTree
template <typename T, typename Allocator>
void BinaryTree<T, Allocator>::deleter() noexcept {
    std::stack<BinaryTree<T, Allocator>::TreeNode*> s;
    BinaryTree<T, Allocator>::TreeNode* last = m_end;

    while (m_root || !s.empty()) {
        while (m_root) {
            s.push(m_root);
            m_root = m_root->left;
        }
        m_root = s.top();
        if (!m_root->right || m_root->right == last) {
            s.pop();
            last = m_root;
            alloc.destroy(m_root);
            alloc.deallocate(m_root, 1);
            m_root = m_end;
        }
        else
            m_root = m_root->right;
    }
}

template<typename T, typename Allocator>
BinaryTree<T, Allocator>::BinaryTree(TreeNode* in_root) : m_root(in_root), m_size(1) {}

template<typename T, typename Allocator>
BinaryTree<T, Allocator>::BinaryTree() : m_root(m_end), m_size(0) {}

template<typename T, typename Allocator>
BinaryTree<T, Allocator>::BinaryTree(BinaryTree<T, Allocator> &&other) noexcept
        : m_root(other.root), m_size(other.size) {
    other.m_root = m_end;
    other.m_size = 0;
}

template<typename T, typename Allocator>
BinaryTree<T, Allocator>::~BinaryTree() {
    deleter();
}

template<typename T, typename Allocator>
BinaryTree<T, Allocator>& BinaryTree<T, Allocator>::operator=(BinaryTree<T, Allocator> &&other) noexcept {
    if (this != &other) {
        deleter(m_root);
        m_root = other.m_root;
        m_size = other.m_size;
        other.m_root = m_end;
        other.m_size = 0;
    }
    return *this;
}



template<typename T, typename Allocator>
std::vector<T> BinaryTree<T, Allocator>::pre_order_traversal() const noexcept {
    if (!m_root)
        return {};

    std::vector<T> order;
    BinaryTree<T, Allocator>::TreeNode* curr = m_root;
    std::stack<BinaryTree<T, Allocator>::TreeNode*> s;

    while (curr || !s.empty()) {
        while (curr) {
            s.push(curr);
            order.push_back(curr->val);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        curr = curr->right;
    }
    return order;
}

template<typename T, typename Allocator>
std::vector<T> BinaryTree<T, Allocator>::in_order_traversal() const noexcept {
    if (!m_root)
        return {};

    std::vector<T> order;
    BinaryTree<T, Allocator>::TreeNode* curr = m_root;
    std::stack<BinaryTree<T, Allocator>::TreeNode*> s;

    while (curr || !s.empty()) {
        while (curr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        order.push_back(curr->val);
        curr = curr->right;
    }
    return order;
}

template<typename T, typename Allocator>
std::vector<T> BinaryTree<T, Allocator>::post_order_traversal() const noexcept {
    if (!m_root)
        return {};

    std::vector<T> order;
    BinaryTree<T, Allocator>::TreeNode* curr = m_root;
    BinaryTree<T, Allocator>::TreeNode* last = m_end;
    std::stack<BinaryTree<T, Allocator>::TreeNode*> s;

    while (curr || !s.empty()) {
        while (curr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        if (!curr->right || curr->right == last) {
            s.pop();
            order.push_back(curr->val);
            last = curr;
            curr = m_end;
        }
        else
            curr = curr->right;
    }
    return order;
}

template<typename T, typename Allocator>
std::vector<T> BinaryTree<T, Allocator>::level_order_traversal() const noexcept {
    std::vector<T> order;
    if (!m_root)
        return {};

    std::queue<TreeNode*> q;
    q.push(m_root);

    while (!q.empty()) {
        auto* current = q.front();
        q.pop();
        order.push_back(current->val);

        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    return order;
}



template<typename T, typename Allocator>
[[nodiscard]] bool BinaryTree<T, Allocator>::is_balanced() const noexcept {
    bool flag = true;
    auto curr = m_root;

    std::function<int(TreeNode*, bool)> checkIsBalanced = [&](TreeNode* curr, bool flag) {
        if (!curr)
            return 0;

        int leftHeight = checkIsBalanced(curr->left, flag);
        int rightHeight = checkIsBalanced(curr->right, flag);

        if (abs(leftHeight - rightHeight) > 1)
            flag = false;

        return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    };

    checkIsBalanced(curr, flag);
    return flag;
}

template<typename T, typename Allocator>
[[nodiscard]] bool BinaryTree<T, Allocator>::is_empty() const noexcept {
    return !m_size;
}

template<typename T, typename Allocator>
[[nodiscard]] bool BinaryTree<T, Allocator>::find(T in_val) const noexcept {
    if (!m_root)
        return false;

    auto curr = m_root;

    while (curr) {
        if (curr->val == in_val)
            return true;
        else if (curr->val < in_val)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return false;
}



template<typename T, typename Allocator>
[[nodiscard]] size_t BinaryTree<T, Allocator>::get_size() const noexcept {
    return m_size;
}

template<typename T, typename Allocator>
[[nodiscard]] int BinaryTree<T, Allocator>::height() const noexcept {
    auto count = 0;
    if (!m_root)
        return count;

    std::stack<TreeNode*> s;
    s.push(m_root);
    TreeNode* root = m_end;

    while(!s.empty()) {
        count++;
        auto stack_size = s.size();
        for (auto i = 0; i < stack_size; i++) {
            root = s.top();
            s.pop();
            if (root->left)
                s.push(root->left);
            if (root->right)
                s.push(root->right);
        }
    }
    return count;
}

template<typename T, typename Allocator>
void BinaryTree<T, Allocator>::inserter(T in_val) noexcept{
    TreeNode* insert_node = alloc.allocate(1);
    alloc.construct(insert_node, TreeNode{in_val, m_end, m_end});

    if (!m_root) {
        m_root = insert_node;
        m_size++;
        return;
    }

    TreeNode *curr = m_root;
    TreeNode *prev = m_end;

    while (curr) {
        if (curr->val == in_val)
            return;
        prev = curr;
        if (curr->val < in_val)
            curr = curr->right;
        else
            curr = curr->left;
    }

    if (prev->val < in_val)
        prev->right = insert_node;
    else
        prev->left = insert_node;
    m_size++;
}



template<typename T, typename Allocator>
void BinaryTree<T, Allocator>::delete_val(T in_val) noexcept{
    if (!find(in_val))
        return;

    if (m_root->val == in_val) {
        alloc.destroy(m_root);
        alloc.deallocate(m_root, 1);
        m_size--;
        return;
    }

    TreeNode *found = m_root;
    TreeNode *prev = m_end;
    std::stack<TreeNode *> m_stack;
    bool last_right = false;

    while (found) {
        m_stack.push(found);
        if (found->val == in_val)
            break;
        if (found->val < in_val) {
            found = found->right;
            last_right = true;
        } else {
            found = found->left;
            last_right = false;
        }
    }

    prev = m_stack.top();
    if (!found->right && !found->left) {
        if (last_right)
            prev->right = m_end;
        else
            prev->left = m_end;
    } else if (!found->right && found->left) {
        TreeNode *next = found->left;

        if (last_right)
            prev->right = next;
        else
            prev->left = next;
    } else if (found->right && !found->right->left) {
        TreeNode *next = found->right;

        if (last_right)
            prev->right = next;
        else
            prev->left = next;
    } else {
        TreeNode *substitute = found;
        TreeNode *prev_substitute = m_end;
        std::stack<TreeNode *> substitute_stack;

        while (substitute) {
            substitute_stack.push(substitute);
            substitute = substitute->left;
        }
        substitute = substitute_stack.top();
        substitute_stack.pop();
        prev_substitute = substitute_stack.top();
        prev_substitute->left = m_end;

        TreeNode* new_node = alloc.allocate(1);
        alloc.construct(new_node, TreeNode{substitute->val, found->left, found->right});

        if (last_right)
            prev->right = new_node;
        else
            prev->left = new_node;

        alloc.destroy(substitute);
        alloc.deallocate(substitute, 1);
        substitute = m_end;
    }

    alloc.destroy(found);
    alloc.deallocate(found, 1);
    found = m_end;
    m_size--;
}



template<typename T, typename Allocator>
typename BinaryTree<T, Allocator>::TreeNode* BinaryTree<T, Allocator>::get_pointer(T in_val) const noexcept{
    if (!m_root)
        return m_end;

    TreeNode *curr = m_root;

    while (curr) {
        if (curr->val == in_val)
            return curr;
        if (curr->val > in_val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return m_end;
}

template<typename T, typename Allocator>
typename BinaryTree<T, Allocator>::TreeNode* BinaryTree<T, Allocator>::get_pointer_min() const noexcept{
    TreeNode *curr = m_root;

    while (curr && curr->left)
        curr = curr->left;
    return curr;
}

template<typename T, typename Allocator>
typename BinaryTree<T,Allocator>::TreeNode* BinaryTree<T,Allocator>::get_pointer_max() const noexcept{
    TreeNode *curr = m_root;

    while (curr && curr->right)
        curr = curr->right;
    return curr;
}

#endif