#include <unistd.h>
#include <fstream>
#include <iostream>
#include <cstring>
int get_memory_by_pid(pid_t pid)
{
    int vmrss = 0;
    char filename[64] = {0};
    snprintf(filename, sizeof(filename), "/proc/%d/statu", pid);
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (ifs.is_open())
    {
        char line[64] = {0};
        while (ifs.getline(line, sizeof(line) - 1))
        {
            if (strstr(line, "VmRSS") != nullptr)
            {
                char name[32] = {0};
                sscanf(line, "%s %d", name, &vmrss);
                break;
            }
        }
    }
    else
    {
        throw std::ios_base::failure("open file fail");
    }
    ifs.close();
    return vmrss;
}