#include "tree.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <string.h>

using steady_clock = std::chrono::steady_clock;

void test_performance()
{
    std::array<int, 10000> randarr;
    for (int i = 1; i <= 10000 ; ++i)
    {
        randarr[i - 1] = i;
    }

    auto root = create_root(0);
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

    // 打乱顺序
    std::random_shuffle(std::begin(randarr), std::end(randarr));

    root = create_root(0);
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
    
    std::array<int, 9> randarr;
    for (int i = 1; i <= 9 ; ++i)
    {
        randarr[i - 1] = i;
    }
    std::random_shuffle(std::begin(randarr), std::end(randarr));

    auto root = create_root(0);
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

    erase(root, 3);
    assert(size(root) == 9);;
    erase(root, 3);
    assert(size(root) == 9);
    erase(root, 7);
    assert(size(root) == 8);
    erase(root, 0);
    assert(size(root) == 7);

    for (auto i : randarr)
    {
        erase(root, i);
    }
    print(root);
    print(root, foreach_type::Mid);
    print(root, foreach_type::Back);
    assert(root->is_valid == false);

}

