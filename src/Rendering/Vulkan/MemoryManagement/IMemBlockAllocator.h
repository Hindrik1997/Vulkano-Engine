//
// Created by hindrik on 4-7-2017.
//

#ifndef VULKANOENGINE_IMEMBLOCKALLOCATOR_H
#define VULKANOENGINE_IMEMBLOCKALLOCATOR_H

#include "MemoryBlock.h"

class IMemBlockAllocator
{
protected:
    MemoryBlock m_MemoryBlock;
public:
    IMemBlockAllocator(MemoryBlock&& memoryBlock);
    IMemBlockAllocator() = delete;
    virtual ~IMemBlockAllocator() = 0;

    IMemBlockAllocator(const IMemBlockAllocator&) = delete;
    IMemBlockAllocator(IMemBlockAllocator&&) = default;

    IMemBlockAllocator& operator=(const IMemBlockAllocator&) = delete;
    IMemBlockAllocator& operator=(IMemBlockAllocator&&) = default;
private:

public:
    typedef uint16_t MemHandle;
    virtual MemHandle allocate(VkDeviceSize size) = 0;
    virtual void free(MemHandle handle) = 0;

    virtual const vk_memory_allocation& operator[](const MemHandle handle) = 0;
};

inline IMemBlockAllocator::IMemBlockAllocator(MemoryBlock&& memoryBlock) : m_MemoryBlock(std::move(memoryBlock))
{

}

inline IMemBlockAllocator::~IMemBlockAllocator()
{
}


#endif //VULKANOENGINE_IMEMBLOCKALLOCATOR_H
