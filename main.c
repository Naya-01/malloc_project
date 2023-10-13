#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Block
{
    size_t size;        
    struct Block *next; 
    struct Block *prev; 
} Block;

Block *allocated_blocks = NULL;

uint8_t MY_HEAP[64000];

void init()
{
}

void *my_malloc(size_t size)
{
    if (size == 0) return NULL;

    Block *current_block = allocated_blocks;

    while (current_block != NULL)
    {
        if (current_block->size >= size)
        {
            current_block->size = size;
            return (uint8_t *)current_block + sizeof(Block);
        }

        current_block = current_block->next;
    }

    // Si aucun bloc libre n'est trouvé
    size_t total_size = size + sizeof(Block);

    if (total_size > sizeof(MY_HEAP)) return NULL;

    Block *new_block = (Block *)MY_HEAP;
    new_block->size = size;
    new_block->next = allocated_blocks;
    new_block->prev = NULL;

    if (allocated_blocks != NULL) allocated_blocks->prev = new_block;
    
    allocated_blocks = new_block;

    return (uint8_t *)new_block + sizeof(Block);
}

void my_free(void *pointer){}

