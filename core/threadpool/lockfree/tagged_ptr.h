#ifndef WISH_TAGGED_PTR
#define WISH_TAGGED_PTR
#include <numeric>
namespace wish
{
template <class T, bool Compression = false>
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

template <class T>
class tagged_ptr<T, true>
{
public:
    using compressed_ptr_t = uint64_t;
    using tag_t = uint16_t;
    tagged_ptr(T *_ptr, tag_t _tag) : m_ptr(pack_ptr(_ptr, _tag))
    {
    }
    tagged_ptr() : m_ptr(pack_ptr(nullptr, 0))
    {
    }
    tagged_ptr(tagged_ptr const &) = default;
    tagged_ptr(tagged_ptr &&) = default;
    tagged_ptr& operator=(tagged_ptr const &) = default;
    tagged_ptr& operator= (tagged_ptr &&) = default;

    bool operator==(volatile tagged_ptr const &other) const
    {
        return m_ptr == other.m_ptr;
    } 

    bool operator!=(volatile tagged_ptr const &other) const
    {
        return !operator==(other);
    } 

    T *get_ptr() const
    {
        return extract_ptr(m_ptr);
    }

    void set_ptr(T *ptr)
    {
        m_ptr = pack_ptr(ptr, get_tag());
    }

    tag_t get_tag() const
    {
        return extract_tag(m_ptr);
    }

    tag_t get_next_tag() const
    {
        return (get_tag() + 1) & std::numeric_limits<tag_t>::max();
    }

    T *operator->() const
    {
        return get_ptr();
    }

    T &operator*() const
    {
        return *get_ptr();
    }

    operator bool() const
    {
        return get_ptr() != nullptr;
    }
private:
    union compressed_ptr_unit
    {
        compressed_ptr_t value;
        tag_t tag[4];
    };

    static compressed_ptr_t pack_ptr(T* ptr, tag_t _tag)
    {
        compressed_ptr_unit ret;
        ret.value = compressed_ptr_t(ptr);
        ret.tag[tag_index] = _tag;

        return ret.value;
    }
    static T* extract_ptr(compressed_ptr_t ptr)
    {
        return (T *)(ptr & ptr_mask);
    }
    static tag_t extract_tag(compressed_ptr_t ptr)
    {
        compressed_ptr_unit ret;
        ret.value = compressed_ptr_t(ptr);
        return ret.tag[tag_index];
    }
    static const uint64_t ptr_mask = (1ull<<48)-1;
    static const size_t tag_index = 3ul; //little edian
    compressed_ptr_t m_ptr;
};

}
#endif