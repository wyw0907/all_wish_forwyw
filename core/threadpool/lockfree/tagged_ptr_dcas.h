namespace wish
{
template <class T>
class tagged_ptr
{
public:
    using tag_t = size_t;
    tagged_ptr(T *_ptr, tag_t _tag) : m_ptr(_ptr), m_tag(_tag)
    {
    }
    tagged_ptr() : m_ptr(nullptr), m_tag(0)
    {
    }
    tagged_ptr(tagged_ptr const &) = default;
    tagged_ptr(tagged_ptr &&) = default;
    tagged_ptr& operator=(tagged_ptr const &) = default;
    tagged_ptr& operator= (tagged_ptr &&) = default;

    bool operator==(volatile tagged_ptr const &other) const
    {
        return (m_ptr == other.m_ptr) && (m_tag == other.m_tag);
    } 

    bool operator!=(volatile tagged_ptr const &other) const
    {
        return !operator==(other);
    } 

    T *get_ptr()
    {
        return m_ptr;
    }

    void set_ptr(T *ptr)
    {
        m_ptr = ptr;
    }

    tag_t get_tag() const
    {
        return m_tag;
    }

    tag_t get_next_tag() const
    {
        return (get_tag() + 1) & std::numeric_limits<tag_t>::max();
    }

    T *operator->() const
    {
        return m_ptr;
    }

    T &operator*() const
    {
        return *m_ptr;
    }

    operator bool() const
    {
        return m_ptr != nullptr;
    }
private:
    tag_t m_tag;
    T * m_ptr;
};


}