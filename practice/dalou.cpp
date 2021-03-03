#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>

int main()
{
    char in[6] = {0};
    int sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0, sum_5 = 0;
    while(std::cin >> in)
    {
        sum_1 += int(in[0] - '0');
        sum_2 += int(in[1] - '0');
        sum_3 += int(in[2] - '0');
        sum_4 += int(in[3] - '0');
        sum_5 += int(in[4] - '0');     
    }
    std::cout << std::min(std::min(std::min(std::min(sum_1, sum_2), sum_3), sum_4),sum_5) << std::endl;
}