#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

void swap(int &a ,int &b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

//构建大顶堆或者小顶堆
void heap_adjust(std::vector<int> &a, size_t end, bool asc)
{
    //堆是一个完全二叉树，从完全二叉树倒数第二列开始取🌲
    // idx = end / 2 是满二叉树的情况，如果不满，前面几个🌲的子节点可能为空，先过滤
    //       1
    //      / \
    //     2   3
    //    / \ / \
    //    4 5 6 7
    for (size_t idx = end / 2; idx >= 1; idx --)
    {
        size_t child_idx_left = idx * 2;
        size_t child_idx_right = idx * 2 + 1;
        
        if (asc)
        {
            if (a[idx-1] < a[child_idx_left - 1])
            {
                swap(a[idx-1], a[child_idx_left - 1]);
            }

            if ((child_idx_right <= end) && (a[idx-1] < a[child_idx_right - 1]))
            {
                swap(a[idx-1], a[child_idx_right - 1]);
            }
        }
        else
        {
             if (a[idx-1] > a[child_idx_left - 1])
            {
                swap(a[idx-1], a[child_idx_left - 1]);
            }

            if ((child_idx_right <= end) && (a[idx-1] > a[child_idx_right - 1]))
            {
                swap(a[idx-1], a[child_idx_right - 1]);
            }
        }
    }

    swap(a[0], a[end - 1]);
}

void heap_sort(std::vector<int> &a, bool asc)
{
    for (size_t end = a.size(); end > 1; --end)
    {
        heap_adjust(a, end, asc);
    }
}

void print(std::vector<int> &a)
{
    std::cout << "[";
    for (auto&v : a)
    {
        std::cout << v << ",";
    }
    std::cout << "]" << std::endl;
}

int main(int argc, char **argv)
{
    int size = 10000;
    if (argc > 1 && argv[1])
    {
        size = atoi(argv[1]);
    }
    std::vector<int> a(size, -1);
    for (int i = 0; i < size ; ++i)
    {
        a[i] = i;
    }
    std::random_shuffle(std::begin(a), std::end(a));
    print(a);
    heap_sort(a, true);
    print(a);
    std::random_shuffle(std::begin(a), std::end(a));
    print(a);
    heap_sort(a, false);
    print(a);

    a.resize(2 * size);
    for (int i = 0; i < size; ++i)
    {
        a[size + i] = std::rand() % size;
    }
    print(a);
    heap_sort(a, true);
    print(a);
    std::random_shuffle(std::begin(a), std::end(a));
    print(a);
    heap_sort(a, false);
    print(a);
}