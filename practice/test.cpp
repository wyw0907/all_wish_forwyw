template<class T>
struct node
{
    T val;
    node *parent;
    node *left;
    node *right;
};

class tree
{
public :
    class iterator
    {
    public :
        iterator(node *p)
        {
            _p = n;
        }
        iterator & operator++()
        {
            if (_p->parent == nullptr)
            {
                _p = _p->left;
            }
            if (nullptr  != _p->parent->left)
            {
                _p = _p->left;
            }
            else
            {
                _p = _p->right;
            }
        }

        iterator operator++(int)
        {

        }
    private:
        node *_p;
    };
};