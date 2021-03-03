/*
腾讯校招题目
小Q想要给他的朋友发送一个神秘字符串，但是他发现字符串的过于长了，
于是小Q发明了一种压缩算法对字符串中重复的部分进行了压缩，对于字符串中连续的m个相同字符串S将会压缩为
[m|S](m为一个整数且1<=m<=100)，例如字符串ABCABCABC将会被压缩为[3|ABC]，
现在小Q的同学收到了小Q发送过来的字符串，你能帮助他进行解压缩么？ 
*/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <algorithm>

std::deque<char>  decompression(char *compress_str)
{
    std::deque<char> deq_compress;
    char *p1 = compress_str;
    while(*p1 != '\0')
    {
        switch(*p1)
        {
        case '[' :
            deq_compress.push_back(*p1++);
            break;
        case ']' :
        {
            int m = 0; //个数
            std::string ss = "";
            while(deq_compress.back() != '[')
            {
                char c = deq_compress.back();
                deq_compress.pop_back();
                ss += c;
            }
            deq_compress.pop_back();
            std::reverse(ss.begin(), ss.end());
            char buf[3] = {0};
            int idx = 0;
            std::string ss2;
            for(auto &c : ss)
            {
                if (m > 0)
                {
                    ss2 += c;
                }
                else if (c == '|')
                {
                    m = atoi(buf);
                }
                else if(c >= '0' && c <= '9')
                {
                    buf[idx++] = c;
                }
            }
            //解压完的数据再导入到栈中
            for (int i = 0 ; i < m ; ++i)
            {
                for(auto &c : ss2)
                {
                    deq_compress.push_back(c);
                }
            }
            p1++;
            break;
        }   
        default :
            deq_compress.push_back(*p1++);
            break;
        }
    }
    return deq_compress;
}

int main()
{
    char input[1000] = {0};
    while (std::cin >> input)
    {
        for (auto &c : decompression(input))
        {
            std::cout << c ;
        }
        std::cout << std::endl;
    }
}

