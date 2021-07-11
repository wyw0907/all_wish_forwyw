#include "rbtree.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <string.h>

using steady_clock = std::chrono::steady_clock;

void test_performance()
{
    std::array<int, 10000> randarr;
    for (int i = 0; i < 10000 ; ++i)
    {
        randarr[i] = i;
    }

    RBTree<int> *root = nullptr;
    auto t1 = steady_clock::now();
    for (auto i : randarr)
    {
        insert(root, i);
    }
    std::cout << "顺序插入10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

    //print(root);
    //print(root, foreach_type::Mid);
    //print(root, foreach_type::Back);

    t1 = steady_clock::now();
    for (auto i : randarr)
    {
        find(root, i);
    }
    std::cout << "顺序查询10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

    t1 = steady_clock::now();
    for (auto i : randarr)
    {
        erase(root, i);
    }
    std::cout << "顺序删除10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

    destroy(root);
    root = nullptr;
    // 打乱顺序
    std::random_shuffle(std::begin(randarr), std::end(randarr));

    t1 = steady_clock::now();
    for (auto i : randarr)
    {
        insert(root, i);
    }
    std::cout << "随机插入10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

    //print(root);
    //print(root, foreach_type::Mid);
    //print(root, foreach_type::Back);
    std::random_shuffle(std::begin(randarr), std::end(randarr));

    t1 = steady_clock::now();
    for (auto i : randarr)
    {
        find(root, i);
    }
    std::cout << "随机查询10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

    std::random_shuffle(std::begin(randarr), std::end(randarr));

    t1 = steady_clock::now();
    for (auto i : randarr)
    {
        erase(root, i);
    }
    std::cout << "随机删除10000次耗时：" << std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now() - t1).count() << std::endl;

}

int main(int ac, char **argv)
{
    if (ac > 1 && strcmp(argv[1], "--performance") == 0)
    {
        test_performance();
        return 0;
    }
    
    std::array<int, 10> randarr;
    for (int i = 0; i < 10 ; ++i)
    {
        randarr[i] = i;
    }
    std::random_shuffle(std::begin(randarr), std::end(randarr));

    RBTree<int> *root = nullptr;
    for (auto i : randarr)
    {
        insert(root, i);
    }
    assert(size(root) == 10);
    print(root);
    print(root, foreach_type::Mid);
    print(root, foreach_type::Back);

    for (auto i : randarr)
    {
        assert(find(root, i)->value == i);
    }

    erase(root, 11);
    assert(size(root) == 10);;


    for (auto i : randarr)
    {
        std::cout << "erase " << i << std::endl;
        erase(root, i);
        print(root);
        print(root, foreach_type::Mid);
        print(root, foreach_type::Back); 
    }
    assert(root == nullptr);

}

