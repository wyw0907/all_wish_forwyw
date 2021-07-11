#include <functional>
#include <iostream>
// 二叉搜索树
template<class T>
struct Tree
{
    Tree<T>* left = nullptr;
    Tree<T>* right = nullptr;
    Tree<T>* parent = nullptr;
    T        value;
    bool     is_valid = true; // 仅针对根结点，表示该根结点是否有效
};

#define IS_ROOT(node) (node->parent == nullptr)

template <class T>
Tree<T> * create_root(const T& t)
{
    Tree<T> * root = new Tree<T>();
    root->value = t;
    return root;
}

template <class T>
Tree<T> * create_root()
{
    Tree<T> * root = new Tree<T>();
    root->is_valid = false;
    return root;
}

template <class T, class F>
void foreach_front(Tree<T> * node, F cb)
{
     // 前序遍历
    if(node == nullptr || !node->is_valid)
    {
        return;
    }
    cb(node);
    foreach_front(node->left, cb);
    foreach_front(node->right, cb);
}

template <class T, class F>
void foreach_middle(Tree<T> * node, F cb)
{
    // 中序遍历
    if(node == nullptr || !node->is_valid)
    {
        return;
    }
    foreach_middle(node->left, cb);
    cb(node);
    foreach_middle(node->right, cb);
}

template <class T, class F>
void foreach_back(Tree<T> * node, F cb)
{
    // 后序遍历
    if(node == nullptr || !node->is_valid)
    {
        return;
    }
    foreach_back(node->left, cb);
    foreach_back(node->right, cb);
    cb(node);
}

enum class foreach_type{
    Front = 1,
    Mid = 2,
    Back = 3,
};

template<class T>
void print(Tree<T>* root, foreach_type type = foreach_type::Front)
{
    std::cout << "[";
    auto cb = [] (Tree<T>* node) {
        std::cout << node->value << ", ";
    };
    switch(type)
    {
        case foreach_type::Front:
            foreach_front(root, cb);
            break;
        case foreach_type::Mid:
            foreach_middle(root, cb);
            break;
        case foreach_type::Back:
            foreach_back(root, cb);
            break;
        default:
            break;
    }
    std::cout << "]" << std::endl;
}

template <class T>
Tree<T>* find(Tree<T>* root, const T& t)
{
    if (root == nullptr || !root->is_valid)
    {
        return nullptr;
    }
    if (root->value == t)
    {
        return root;
    }
    else if (root->value > t)
    {
        return find(root->left, t);
    }
    else
    {
        return find(root->right, t);
    }
}


template <class T>
void insert(Tree<T>* root, const T& t)
{
    if (!root->is_valid)
    {
        root->is_valid = true;
        root->value = t;
        return;
    }
    else if (t == root->value)
    {
        return; // 重复值抛弃
    }
    else if (root->value > t)
    {
        if (root->left == nullptr)
        {
            Tree<T> * nnode = new Tree<T>();
            nnode->value = t;
            nnode->parent = root;
            root->left = nnode;
            return;
        }
        insert(root->left, t);
    }
    else
    {
        if (root->right == nullptr)
        {
            Tree<T> * nnode = new Tree<T>();
            nnode->value = t;
            nnode->parent = root;
            root->right = nnode;
            return;
        }
        insert(root->right, t);
    }
}

template <class T>
void erase(Tree<T>* node)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        if(!IS_ROOT(node))
        {
            if(node->parent->left == node)
            {
                node->parent->left = nullptr;
            }
            else
            {
                node->parent->right = nullptr;
            }
            delete node;
            node = nullptr;
        }
        else
        {
            //delete node;
            //node = nullptr;
            // 此时根结点不可用了，怎么办
            node->is_valid = false;
        }
    }
    else if (node->left == nullptr || node->right == nullptr)
    {
        auto next = node->left ? node->left : node->right;
        if(!IS_ROOT(node))
        {
            if (node->parent->left == node)
            {
                node->parent->left = next;
            }
            else
            {
                node->parent->right = next;
            }
            next->parent = node->parent;
            delete node;
            node = nullptr;
        }
        else
        {
            // next->parent = nullptr;
            // root = next;  //替换root
            //直接替换的话需要外部传入二级指针，这里替换值
            node->value  = std::move(next->value);
            node->left = next->left;
            node->right = next->right;
            if (next->left)
            {
                next->left->parent = node;
            }
            if (next->right)
            {
                next->right->parent = node;
            }
            delete next;
            next = nullptr;
        }
        
    }
    else
    {
        // 从左子树找到最大叶子或者从右子树找到最小叶子代替
        // 这里选择第一种，使用最简单的方法，替换值，指针不变
        auto last = node->left;
        while(last->right)
        {
            last = last->right;
        }
        node->value = std::move(last->value);
        if (last->parent == node)
        {
            last->parent->left = last->left;
            if (last->left)
            {
                last->left->parent = last->parent;
            }
        }
        else
        {
            last->parent->right = last->left;
            if (last->left)
            {
                last->left->parent = last->parent;
            }
        }
        delete last;
        last = nullptr;
    }
}

template <class T>
void erase(Tree<T>* node, const T& t)
{
    if (node == nullptr || !node->is_valid)
    {
        return;
    }
    else if (t == node->value)
    {
        erase(node);
    }
    else if (node->value > t)
    {
        erase(node->left, t);
    }
    else
    {
        erase(node->right, t);
    }
}


template <class T>
int size(Tree<T>* root)
{
    int cnt = 0;
    auto cb = [&cnt] (Tree<T>* node) {
        cnt ++;;
    };
    foreach_back(root, cb);
    return cnt;
}


template <class T>
void destroy(Tree<T>* root)
{
    auto cb = [] (Tree<T>* node) {
        delete node;
    };
    foreach_back(root, cb);
}
