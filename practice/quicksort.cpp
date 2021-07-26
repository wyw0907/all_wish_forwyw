#include <cstdio>
#include <unistd.h>
#include <cstdlib>
void swap(int& x , int& y)
{
    x ^= y;
    y ^= x;
    x ^= y;
}

void quick_sort(int *arr, int begin, int end)
{
    if (begin >= end)
    {
        return ;
    }
    int left = begin + 1, right = end;
    int middle = begin;
    for(;;)
    {
        while (arr[left] < arr[middle])
        {
            if (left >= end)
            {
                break;
            }
            ++left;
        }
        while (arr[right] > arr[middle])
        {
            if (right <= begin)
            {
                break;
            }
            --right;
        }
        if (left >= right)
        {
            break;
        }
        swap(arr[left], arr[right]);
    }

    swap(arr[middle], arr[right]);

    quick_sort(arr, begin, right - 1);
    quick_sort(arr, right + 1, end);
}


int main()
{
    int arr[10] = {1,3,4,5,7,9,8,6,0,2};
    quick_sort(arr, 0, 9);

    for (int idx = 0; idx < 10; ++idx)
    {
        printf("%d, ", arr[idx]);
    }
    printf("\n");
}