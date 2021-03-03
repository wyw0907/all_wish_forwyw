#include "string_util.h"
namespace wish
{
    int atoi(const std::string &s)
    {
        int ret = 0;
        if (s.empty()) return ret;
        int skip = 0;
        int idx = 0;
        while (idx < s.size())
        {
            if (s.at(idx) != ' ') break;
            skip++, idx++;
        }
        if (skip >= s.size()) return ret;
        char head = s.at(skip);
        bool flag = false;
        if (head != '+' && head != '-' && (head > '9' || head < '0')) return ret;
        if (head == '+') 
        {
            skip++;
        }
        else if (head == '-')
        {
            flag = true;
            skip++;
        }
        for (int i = skip; i < s.size(); i++)
        {
            const char c = s.at(i);
            if (c > '9' || c < '0') break;
            int n = c - '0';
            if (ret > (int(0x7fffffff)) / 10 - n)
            {
                ret = flag ? (-0x7fffffff - 1) : 0x7fffffff;
                break;
            }
            ret = ret * 10 + n;
        }
        return (flag && ret != (-0x7fffffff - 1)) ? -ret : ret;
    }




} // namespace wish