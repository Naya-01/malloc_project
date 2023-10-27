#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t MY_HEAP[64000];

typedef struct block_header
{
    size_t size;
    int8_t is_free;
    int8_t is_initialized;
    struct block_header *next;
    struct block_header *prev;
} BlockHeader;


void init()
{
    BlockHeader *heap_base = (BlockHeader *)MY_HEAP;
    heap_base->size = sizeof(MY_HEAP) - sizeof(BlockHeader);
    heap_base->is_free = 1;
    heap_base->is_initialized = 1;
    heap_base->next = NULL;
    heap_base->prev = NULL;
}

BlockHeader *find_free_block(size_t size)
{
    BlockHeader *heap_base = (BlockHeader *)MY_HEAP;
    BlockHeader *current = heap_base;
    while (current)
    {
        if (current->is_free && current->size >= size)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void *my_malloc(size_t size)
{
    BlockHeader *heap_base = (BlockHeader *)MY_HEAP;
    if (!heap_base->is_initialized)// Si le heap n'est pas initialisé
    { 
        init();
    }

    BlockHeader *block = find_free_block(size);
    if (block)
    {
        if (block->size > size + sizeof(BlockHeader))// Si le bloc trouvé est suffisamment grand pour être divisé
        { 
            BlockHeader *new_block = (BlockHeader *)((uint8_t *)block + sizeof(BlockHeader) + size);
            new_block->size = block->size - size - sizeof(BlockHeader);
            new_block->is_free = 1;
            new_block->next = block->next;
            new_block->prev = block;

            if (block->next)
            {
                block->next->prev = new_block; 
            }

            block->size = size;
            block->next = new_block;
        }
        block->is_free = 0;
        return (uint8_t *)block + sizeof(BlockHeader);
    }

    return NULL; 
}

void my_free(void *pointer)
{
    if (!pointer)
    {
        return;
    }

    BlockHeader *block = (BlockHeader *)((uint8_t *)pointer - sizeof(BlockHeader));
    block->is_free = 1;

    // Fusion avec le bloc suivant si celui-ci est libre
    BlockHeader *next_block = block->next;
    if (next_block && next_block->is_free)
    {
        block->size += sizeof(BlockHeader) + next_block->size;
        block->next = next_block->next;
        if (next_block->next)
        {
            next_block->next->prev = block;
        }
    }

    // Fusion avec le bloc précédent
    BlockHeader *prev_block = block->prev;
    if (prev_block && prev_block->is_free)
    {
        prev_block->size += sizeof(BlockHeader) + block->size;
        prev_block->next = block->next;
        if (block->next)
        {
            block->next->prev = prev_block;
        }
    }
}

int main()
{
    init();
    int *int_ptr = (int *)my_malloc(sizeof(int));
    double *double_ptr = (double *)my_malloc(sizeof(double));
    char *char_ptr = (char *)my_malloc(10*sizeof(char));

    if (int_ptr)
    {
        *int_ptr = 42;
        printf("Int: %zu bytes\n", sizeof(int));
    }

    if (double_ptr)
    {
        *double_ptr = 3.14159;
        printf("Double: %zu bytes\n", sizeof(double));
    }

    if (char_ptr)
    {
        snprintf(char_ptr, 10, "Hello");
        printf("Char: %zu bytes\n", 10*sizeof(char));
    }

    my_free(int_ptr);
    my_free(double_ptr);
    my_free(char_ptr);

    return 0;
}
