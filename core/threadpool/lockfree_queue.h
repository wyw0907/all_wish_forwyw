#ifndef WISH_LOCKFREE_QUEUE
#define WISH_LOCKFREE_QUEUE
namespace wish
{

static 

namespace {
    template <class T>
    struct node
    {
        T m_value;
        uint32_t m_counter;
    };

}


template<class Item>
class lockfree_queue
{
public:
    using value_type = Item;
    // pop与front保持原子性
    value_type* pop_front()
    {
        Item *_item = nullptr;
        while(!m_list.empty())
        {

        }
        return _item;
    }

    size_t size() const
    {
        return m_list.size();
    }

    bool empty() const
    {
        return size() == 0;
    }

    void emplace(Item&& item)
    {
        Item *_item = new Item(std::move(item));
        m_list.emplace_back(_item);
    }

    void emplace(const Item& item)
    {
        Item *_item = new Item(item);
        m_list.emplace_back(_item);
    }

private:
    std::list<Item *>    m_list;
};

}

#endif