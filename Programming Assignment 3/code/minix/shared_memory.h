// shared_memory.h
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define SHM_KEY 1234
#define SHM_SIZE sizeof(shared_memory_t)

typedef struct {
    int lock; // Âê
    char message[100]; // °T®§

} shared_memory_t;

#endif
