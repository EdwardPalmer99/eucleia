/**
 * @file PoolAllocator.hpp
 * @author Edward Palmer
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <new>
#include <cstddef>

struct Chunk
{
    Chunk *next{nullptr};
};

/**
 * The allocator class.
 *
 * - Consider it as consisting of a series of large blocks with smaller chunks.
 *
 * Ref: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
 */
class PoolAllocator
{
public:
    PoolAllocator(size_t chunksPerBlock_) : chunksPerBlock(chunksPerBlock_) {}

    void *allocate(size_t chunkSize);

    void deallocate(void *ptr);

private:
    Chunk *allocateBlock(size_t chunkSize);

    /**
     * Number of chunks per block.
     */
    size_t chunksPerBlock;

    /**
     * Allocation pointer.
     */
    Chunk *nextAvailChunk{nullptr};
};