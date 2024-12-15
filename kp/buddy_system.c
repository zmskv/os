#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

typedef struct BuddyBlock
{
    size_t size;
    int free;
    struct BuddyBlock *buddy;
} BuddyBlock;

typedef struct BuddyAllocator
{
    void *memory;
    size_t memory_size;
    BuddyBlock *blocks;
} BuddyAllocator;

size_t roundUpToPowerOfTwo(size_t size)
{
    return pow(2, ceil(log2(size)));
}

BuddyAllocator *createBuddyAllocator(void *realMemory, size_t memory_size)
{
    BuddyAllocator *allocator = (BuddyAllocator *)malloc(sizeof(BuddyAllocator));
    allocator->memory_size = roundUpToPowerOfTwo(memory_size);
    allocator->memory = realMemory;
    allocator->blocks = (BuddyBlock *)realMemory;

    allocator->blocks->size = allocator->memory_size;
    allocator->blocks->free = 1;
    allocator->blocks->buddy = NULL;

    return allocator;
}

void *allocBuddy(BuddyAllocator *allocator, size_t block_size)
{
    block_size = roundUpToPowerOfTwo(block_size);

    BuddyBlock *current = allocator->blocks;

    while (current && current->size < block_size)
    {
        current = current->buddy;
    }

    if (current && current->free)
    {
        current->free = 0;
        return (char *)current + sizeof(BuddyBlock);
    }

    return NULL;
}

void freeBuddy(BuddyAllocator *allocator, void *ptr)
{
    BuddyBlock *block_to_free = (BuddyBlock *)((char *)ptr - sizeof(BuddyBlock));
    block_to_free->free = 1;
}
