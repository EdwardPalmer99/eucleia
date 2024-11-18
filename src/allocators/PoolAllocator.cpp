/**
 * @file PoolAllocator.cpp
 * @author Edward Palmer
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "PoolAllocator.hpp"
#include <cstdlib>
#include <iostream>

// TODO: - need some way to cleanup and destroy old blocks if memory goes way down.

Chunk *PoolAllocator::allocateBlock(size_t chunkSize)
{
    // static size_t blockCounter = 0;
    // std::cout << "allocating new block; blockCounter = " << ++blockCounter << std::endl;

    size_t blockSize = (chunkSize * chunksPerBlock);

    Chunk *blockBegin = reinterpret_cast<Chunk *>(malloc(blockSize));
    Chunk *chunk = blockBegin;

    for (int i = 0; i < (chunksPerBlock - 1); ++i)
    {
        chunk->next = reinterpret_cast<Chunk *>((uint8_t *)chunk + chunkSize);
        chunk = chunk->next;
    }

    chunk->next = nullptr;

    return blockBegin;
}


void *PoolAllocator::allocate(size_t chunkSize)
{
    // std::cout << "allocating chunk: " << ++numChunksInUse << std::endl;

    if (!nextAvailChunk) // No blocks allocated or no chunks available.
    {
        nextAvailChunk = allocateBlock(chunkSize);
    }

    // TODO: - check chunkSize matches what we expect here.

    Chunk *ptr = nextAvailChunk;

    // Advance to next chunk. When no available chunks, it will be set to nullptr;
    nextAvailChunk = nextAvailChunk->next;

    return ptr;
}


void PoolAllocator::deallocate(void *chunk)
{
    // std::cout << "deallocating chunk: " << --numChunksInUse << std::endl;

    // Freed pointer's next pointer points to current allocation pointer and
    // allocation pointer set to returned free chunk.
    reinterpret_cast<Chunk *>(chunk)->next = nextAvailChunk;

    nextAvailChunk = reinterpret_cast<Chunk *>(chunk);
}