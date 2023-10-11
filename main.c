#include <stdint.h>
#include <stdlib.h>

typedef struct Block
{
    size_t size;
    struct Block *prev;
    struct Block *next;
} Block;

uint8_t MY_HEAP[64000];

void init();
void *my_malloc(size_t size);
void my_free(void *ptr);
