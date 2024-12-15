#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct Block
{
    size_t size;
    struct Block *next;
} Block;

typedef struct FreeListAllocator
{
    Block *free_list;
    void *memory;
    size_t memory_size;
} FreeListAllocator;

FreeListAllocator *createMemoryAllocator(void *realMemory, size_t memory_size)
{
    FreeListAllocator *allocator = (FreeListAllocator *)malloc(sizeof(FreeListAllocator));
    allocator->memory = realMemory;
    allocator->memory_size = memory_size;

    allocator->free_list = (Block *)realMemory;
    allocator->free_list->size = memory_size;
    allocator->free_list->next = NULL;

    return allocator;
}

void *alloc(FreeListAllocator *allocator, size_t block_size)
{
    Block *prev = NULL;
    Block *current = allocator->free_list;

    while (current)
    {
        if (current->size >= block_size)
        {
            if (current->size > block_size + sizeof(Block))
            {

                Block *new_block = (Block *)((char *)current + block_size);
                new_block->size = current->size - block_size;
                new_block->next = current->next;

                current->size = block_size;
                current->next = new_block;
            }

            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                allocator->free_list = current->next;
            }

            return (char *)current + sizeof(Block);
        }
        prev = current;
        current = current->next;
    }

    return NULL;
}

void freeBlock(FreeListAllocator *allocator, void *ptr)
{
    Block *block_to_free = (Block *)((char *)ptr - sizeof(Block));
    block_to_free->next = allocator->free_list;
    allocator->free_list = block_to_free;
}
