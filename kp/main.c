#include <stdio.h>
#include "tests.c"

int main()
{
    printf("Testing Free List Allocator\n");
    testFreeListAllocatorPerformance();
    printf("Testing Buddy Allocator\n");
    testBuddyAllocatorPerformance();

    return 0;
}