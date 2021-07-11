#include <iostream>
#include <thread>
#include <chrono>

enum class ColorType {
    Red,
    Black
};

template<class T>
struct RBTree
{
    RBTree *parent;
    RBTree *left;
    RBTree *right;
    ColorType   color;
    T value;

    void set_red()
    {
        color = ColorType::Red;
    }

    void set_black()
    {
        color = ColorType::Black;
    }

    bool is_black() const
    {
        return color == ColorType::Black;
    }

    bool is_red() const
    {
        return color == ColorType::Red;
    }
};

template <class T, class F>
void foreach_front(RBTree<T> * node, F cb)
{
     // 前序遍历
    if(node == nullptr)
    {
        return;
    }
    cb(node);
    foreach_front(node->left, cb);
    foreach_front(node->right, cb);
}

template <class T, class F>
void foreach_middle(RBTree<T> * node, F cb)
{
    // 中序遍历
    if(node == nullptr)
    {
        return;
    }
    foreach_middle(node->left, cb);
    cb(node);
    foreach_middle(node->right, cb);
}

template <class T, class F>
void foreach_back(RBTree<T> * node, F cb)
{
    // 后序遍历
    if(node == nullptr)
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
void print(RBTree<T>* root, foreach_type type = foreach_type::Front)
{
    std::cout << "[";
    auto cb = [] (RBTree<T>* node) {
        std::cout << node->value <<  (node->is_red() ? "R" : "B") << ", ";
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
RBTree<T>* find(RBTree<T>* root, const T& t)
{
    if (root == nullptr)
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
int size(RBTree<T>* root)
{
    int cnt = 0;
    auto cb = [&cnt] (RBTree<T>* node) {
        cnt ++;;
    };
    foreach_back(root, cb);
    return cnt;
}


template <class T>
void destroy(RBTree<T>* root)
{
    auto cb = [] (RBTree<T>* node) {
        delete node;
    };
    foreach_back(root, cb);
}

template <class T>
void left_revolve(RBTree<T> *&root, RBTree<T> *x)
{
    auto y = x->right; 
    x->right = y->left;
    if (y->left != nullptr)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if( x->parent == nullptr )
    {
        root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template <class T>
void right_revolve(RBTree<T> *&root, RBTree<T> *y)
{
    auto x = y->left; 
    y->left = x->right;
    if (x->right)
    {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if( y->parent == nullptr )
    {
        root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else if (y == y->parent->right)
    {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

template<class T>
void insert_fixup(RBTree<T> *&root,  RBTree<T> *node)
{
    RBTree<T> *p, *g, *u;
    while((p = node->parent) && p->is_red())
    {
        g = p->parent;
        if (p == g->left)
        {
            // case 1
            u = g->right;
            if (u && u->is_red())
            {
                u->set_black();
                p->set_black();
                g->set_red();
                node = g;
                continue;
            }

            // case 2
            if (p->right == node)
            {
                left_revolve(root, p);
                node = p;
            }

            // case3
            if (p->left == node)
            {
                p->set_black();
                g->set_red();
                right_revolve(root, g);
            }
        }
        else
        {
            // case 1
            u = g->left;
            if (u && u->is_red())
            {
                u->set_black();
                p->set_black();
                g->set_red();
                node = g;
                continue;
            }

            // case 2
            if (p->left == node)
            {
                RBTree<T> * tmp = nullptr;
                right_revolve(root, p);
                tmp = p;
                p = node;
                node = tmp;
            }

            // case3 上下使用了两种写法，两种都可以，直接换或者重新循环
            p->set_black();
            g->set_red();
            left_revolve(root, g);
        }
    }
    root->set_black();
    return;
}

template <class T>
void insert(RBTree<T>* &root, RBTree<T>* node)
{
    RBTree<T> *y = nullptr;
    RBTree<T> *x = root;
    while( x != nullptr)
    {
        y = x;
        if (node->value < x->value)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    node->parent= y;
    if (y != nullptr)
    {
        if (node->value < y->value)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }
    }
    else
    {
        root = node;
    }
    node->set_red();
    insert_fixup(root, node);
}

template <class T>
void insert(RBTree<T>* &root, const T& t)
{
    auto node = new RBTree<T>();
    node->value = t;
    insert(root, node);
}

template <class T>
void erase_fixup(RBTree<T>* &root, RBTree<T>* node, RBTree<T>* parent)
{
    RBTree<T> *s;
    // 当前节点是根节点或者红色节点时退出
    while(node != root && (!node || node->is_black()))
    {
        // 核心思路，从兄弟那条分支借一个红色节点,让它变黑色
        // 如果兄弟就是红的，通过旋转把它变黑
        // 如果兄弟两个孩子都是黑的，把问题交给父亲节点解决
        if (node == parent->left)
        {
            s = parent->right;
            // case 1 如果兄弟是红色，那么左旋，同时把parent改成红色，兄弟改成黑色
            // 修改之后新的兄弟节点就是黑色，其他数据都没有变化，开始流程case 2 3
            if (s->is_red())
            {
                s->set_black();
                parent->set_red();
                left_revolve(root, parent);
                continue;
            }
            
            // case2 兄弟节点没有红色的孩子，把问题交给父亲解决
            if ((!s->left || !s->left->is_red()) && (!s->right || !s->right->is_red()))
            {
                s->set_red();
                node = parent;
                parent = node->parent;
                continue;
            }

            // case 3 红色节点在反位置，利用旋转换到另一边
            if (s->left && s->left->is_red() && (!s->right || s->right->is_black()))
            {
                s->left->set_black();
                s->set_red();
                right_revolve(root, s);
                continue;
            }

            // case 4 兄弟的右孩子是红色，直接借过来
            if (s->right && s->right->is_red())
            {
                s->color = parent->color;
                parent->set_black(); //让被删除的黑色节点加回来
                s->right->set_black(); //红变黑 保持平衡
                left_revolve(root, parent);
                break;
            }
        }
        else //反转上面的逻辑即可
        {
            s = parent->left;
            if (s->is_red())
            {
                s->set_black();
                parent->set_red();
                right_revolve(root, parent);
                continue;
            }
            
            if ((!s->left || !s->left->is_red()) && (!s->right || !s->right->is_red()))
            {
                s->set_red();
                node = parent;
                parent = node->parent;
                continue;
            }

            if (s->right && s->right->is_red() && (!s->left || s->left->is_black()))
            {
                s->right->set_black();
                s->set_red();
                left_revolve(root, s);
                continue;
            }

            if (s->left && s->left->is_red())
            {
                s->color = parent->color;
                parent->set_black(); //让被删除的黑色节点加回来
                s->left->set_black(); //红变黑 保持平衡
                right_revolve(root, parent);
                break;
            }
        }
    }
    if (node)
    {
        node->set_black();
    }
}

template <class T>
void erase(RBTree<T>* &root, RBTree<T>* node)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        if (node->parent == nullptr)
        {
            delete root;
            root = nullptr;
            return;
        }
        if (node->parent->left == node)
        {
            node->parent->left = nullptr;
        }
        else
        {
            node->parent->right = nullptr;
        }
        if (node->is_black())
        {
            // 被删除节点是黑色,修复红黑树
            erase_fixup(root, (RBTree<T>*)nullptr, node->parent);
        } 
        delete node;
    }
    else if (node->left == nullptr || node->right == nullptr)
    {
        // 待删除节点的子节点
        auto child = node->left ? node->left : node->right;
        if(node->parent == nullptr)
        {
            root = child;
        }
        else
        {
            if (node->parent->left == node)
            {
                node->parent->left = child;
            }
            else
            {
                node->parent->right = child;
            }
        }
        child->parent = node->parent;
        if (node->is_black())
        {
            // 被删除节点是黑色,修复红黑树
            erase_fixup(root, child, node->parent);
        } 
        delete node;
    }
    else
    {
        // 找到右边的最小节点
        auto replace = node->right;
        while(replace->left)
        {
            replace = replace->left;
        }

         // 待删除节点的子节点
        auto child = replace->right;
        auto parent = replace->parent;
        // 被删除的颜色
        auto color = replace->color;
        if (replace->parent == node)
        {
            //因为等下replace会替换node, 所以不用动
            parent = replace;
        }
        else
        {
            // 将replace的右孩子指向replace的parent
            if (replace->right)
            {
                replace->right->parent = replace->parent;
            }
            replace->parent->left = replace->right;
            // 将node的右孩子指向replace
            replace->right = node->right;
            node->right->parent = replace;
        }
        // 将node的左孩子指向replace
        replace->left = node->left;
        node->left->parent = replace;

        // 将replace的父母改成node的父母
        if(node->parent == nullptr)
        {
            root = replace;
        }
        else
        {
            if (node->parent->left == node)
            {
                node->parent->left = replace;
            }
            else
            {
                node->parent->right = replace;
            }
        }
        replace->parent = node->parent;
        // 拷贝颜色
        replace->color = node->color;

        if (color == ColorType::Black)
        {
            // 被删除节点是黑色,修复红黑树
            erase_fixup(root, child, parent);
        }
        delete node;
    }
}

template <class T>
void erase(RBTree<T>* &root, const T& t)
{
    auto node = find(root, t);
    if (node != nullptr)
    {
        erase(root, node);
    }
}