//
// Created by hindrik on 16-6-17.
//

#include "MemoryManager.h"
#include "../PresentDevice.h"
#include "../../Utilities/VulkanUtilityFunctions.h"

const VkDeviceSize _16KB  = 16384;
const VkDeviceSize _32KB  = 32768;
const VkDeviceSize _64KB  = 65536;
const VkDeviceSize _128KB = 131072;
const VkDeviceSize _256KB = 262144;
const VkDeviceSize _512KB = 524288;
const VkDeviceSize _1MB   = 1048576;
const VkDeviceSize _2MB   = 2097152;
const VkDeviceSize _4MB   = 4194304;
const VkDeviceSize _8MB   = 8388608;
const VkDeviceSize _16MB  = 16777216;
const VkDeviceSize _32MB  = 33554432;
const VkDeviceSize _64MB  = 67108864;
const VkDeviceSize _128MB = 134217728;
const VkDeviceSize _256MB = 268435456;
const VkDeviceSize _512MB = 536870912;

template <typename T>
void moveAppend(std::vector<T> &src, std::vector<T> &dst)
{
    if (dst.empty())
    {
        dst = std::move(src);
    }
    else
    {
        dst.reserve(dst.size() + src.size());
        std::move(std::begin(src), std::end(src), std::back_inserter(dst));
        src.clear();
    }
}

MemoryManager::MemoryManager(PresentDevice& presentDevice) : m_Device(&presentDevice)
{
    initializeMemorySystem();
//
//    vector<uint16_t> handles;
//
//    for(uint16_t i = 0; i < 256; ++i)
//        handles.push_back(m_Allocators.front().allocate(_1MB));
//
//    Logger::log(std::to_string(handles.size()));
//    for(auto& i : handles)
//    {
//        Logger::log(std::to_string(i));
//    }
//    for(uint16_t i = 0; i < 256; ++i)
//    {
//        m_Allocators.back().free(handles[i]);
//    }
//    Logger::log("break");
}

vector<vk_memory_heap> MemoryManager::getMemoryInfo(VkPhysicalDevice device)
{
    vector<vk_memory_heap> heaps = {};

    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    for(uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
    {

        vk_memory_heap heap = {};
        heap.m_MemoryHeap = memoryProperties.memoryHeaps[i];

        for(uint32_t j = 0; j < memoryProperties.memoryTypeCount; ++j)
        {
            if(memoryProperties.memoryTypes[j].heapIndex == i && memoryProperties.memoryTypes[j].propertyFlags != 0x0)
            {
                heap.m_MemoryTypes.push_back(make_pair(static_cast<VkMemoryPropertyFlagBits>(memoryProperties.memoryTypes[j].propertyFlags), j));
            }
        }

        heap.m_IsDeviceLocal = (heap.m_MemoryHeap.flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
        heaps.push_back(heap);
    }

    return heaps;
}

vector<MemoryBlock> MemoryManager::allocateMemblocks(VkDevice device, const vk_memory_heap& heap, VkDeviceSize allocateSize, uint32_t memoryTypeIndex, uint32_t blockCount)
{
    vector<MemoryBlock> blocks;

    for(uint32_t i = 0; i < blockCount; ++i)
    {
        VkMemoryAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.memoryTypeIndex = memoryTypeIndex;
        info.allocationSize  = allocateSize;

        VkDeviceMemory memory;
        VkResult result = vkAllocateMemory(device, &info, nullptr, &memory);

        if(result != VK_SUCCESS)
        {
            Logger::warn("Error allocating memoryblock.");
            break;
        }
        blocks.emplace_back(device, allocateSize, memoryTypeIndex, std::move(memory));
    }

    return blocks;
}

vector<MemoryBlock> MemoryManager::fillMemoryHeap(VkDevice device, const vk_memory_heap& heap, VkDeviceSize allocateSize, uint32_t memoryTypeIndex, VkDeviceSize& alreadyAllocatedAmount)
{
    VkResult result;
    uint32_t bufferCount = 0;
    uint32_t fitCount = static_cast<uint32_t>(heap.m_MemoryHeap.size / allocateSize);

    vector<MemoryBlock> blocks;

    for(uint32_t i = 0; i < fitCount; ++i)
    {
        if(alreadyAllocatedAmount >= heap.m_MemoryHeap.size)
            break;

        VkMemoryAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.memoryTypeIndex = memoryTypeIndex;
        info.allocationSize  = allocateSize;

        VkDeviceMemory memory;
        result = vkAllocateMemory(device, &info, nullptr, &memory);

        if(result != VK_SUCCESS)
        {
            Logger::warn("Error allocating memoryblock.");
            break;
        }
        alreadyAllocatedAmount += allocateSize;
        bufferCount++;
        blocks.emplace_back(device, allocateSize, memoryTypeIndex, std::move(memory));
    }

    return blocks;
}

void MemoryManager::initializeMemorySystem()
{
    vector<vk_memory_heap> memHeaps = getMemoryInfo(m_Device->physicalDevice());

    for(const auto& heap : memHeaps)
    {
        if(heap.m_IsDeviceLocal)
            setupDeviceLocalHeap(heap);
        else
            setupHostLocalHeap(heap);
    }
}

void MemoryManager::setupDeviceLocalHeap(const vk_memory_heap &heap)
{
    VkDeviceSize allocatedAmount = 0;

    bool hasHostVisible = false;
    bool hasDeviceLocal = false;
    uint32_t hostVisibleIndex = 0;
    uint32_t deviceLocalIndex = 0;
    for(const auto& mType : heap.m_MemoryTypes)
    {
        if(mType.first & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && mType.first & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        {
            hasHostVisible = true;
            hostVisibleIndex = mType.second;
        }
        if(mType.first & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && !(mType.first & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
        {
            hasDeviceLocal = true;
            deviceLocalIndex = mType.first;
        }
    }

    if(hasHostVisible && hasDeviceLocal)
    {
        //Allocate 4 Host visible buffers.
        vector<MemoryBlock> hostVisibleBlocks = allocateMemblocks(m_Device->device(), heap, _64MB, hostVisibleIndex, 4);
        allocatedAmount += (static_cast<uint32_t>(hostVisibleBlocks.size()) * _64MB);
        vector<MemoryBlock> deviceLocalBlocks = fillMemoryHeap(m_Device->device(), heap, _256MB, deviceLocalIndex, allocatedAmount);

        Logger::log("Allocated " + std::to_string(hostVisibleBlocks.size()) + " 64MB buffers on memory heap " + std::to_string(heap.m_HeapIndex) + " (VRAM)");
        Logger::log("Allocated " + std::to_string(deviceLocalBlocks.size()) + " 256MB buffers on memory heap " + std::to_string(heap.m_HeapIndex) + " (VRAM)");

        moveAppend(hostVisibleBlocks, m_DeviceLocalHostVisibleMemoryBlocks);
        moveAppend(deviceLocalBlocks, m_DeviceLocalMemoryBlocks);
        //Done allocating VRAM!
    }
    else
    {
        if(!hasHostVisible && hasDeviceLocal)
        {
            Logger::log("Found no host mappable VRAM!");

            vector<MemoryBlock> deviceLocalBlocks = fillMemoryHeap(m_Device->device(), heap, _256MB, deviceLocalIndex, allocatedAmount);
            Logger::log("Allocated " + std::to_string(deviceLocalBlocks.size()) + " 256MB buffers on memory heap " + std::to_string(heap.m_HeapIndex) + " (VRAM)");
            moveAppend(deviceLocalBlocks, m_DeviceLocalMemoryBlocks);
            //Done allocating VRAM!
        }
        else
        {
            Logger::failure("An error occurred during memory allocation!");
        }
    }
    m_DeviceLocalMemoryBlocks.shrink_to_fit();
    m_DeviceLocalHostVisibleMemoryBlocks.shrink_to_fit();
}

void MemoryManager::setupHostLocalHeap(const vk_memory_heap &heap)
{
    vector<pair<VkMemoryPropertyFlagBits, uint32_t>> hostCoherentTypes;
    vector<pair<VkMemoryPropertyFlagBits, uint32_t>> hostCachedTypes;
    vector<pair<VkMemoryPropertyFlagBits, uint32_t>> hostCachedhostCoherentTypes;

    for(const auto& type : heap.m_MemoryTypes)
    {
        if(!(type.first & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
            continue;

        if(type.first & VK_MEMORY_PROPERTY_HOST_CACHED_BIT && type.first & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
            hostCachedhostCoherentTypes.push_back(type);
        else if (type.first & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT && !(type.first & VK_MEMORY_PROPERTY_HOST_CACHED_BIT))
        {
            hostCoherentTypes.push_back(type);
        } else
        {
            assert(!(type.first & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) && type.first & VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
            hostCachedTypes.push_back(type);
        }

    }

    //In each case allocate a 256MB buffer...

    bool downloadbuffer = false;
    if(hostCachedhostCoherentTypes.size() > 0)
    {
        //Use for downloads...
        for(const auto& type : hostCachedhostCoherentTypes)
        {
            vector<MemoryBlock> hostCachedhostCoherent = allocateMemblocks(m_Device->device(), heap, _256MB, type.second, 1);
            if(hostCachedhostCoherent.size() > 0)
            {
                downloadbuffer = true;
                Logger::log("Allocated " + std::to_string(char(1)) + " 256MB HostLocalHostCoherent buffer for downloads");
                moveAppend(hostCachedhostCoherent, m_HostLocalHostCoherentHostCachedMemoryBlocks);
                break;
            }
        }


    }

    if(hostCachedTypes.size() > 0)
    {
        //Prefer host cached for uploads...
        for(const auto& type : hostCachedTypes)
        {
            vector<MemoryBlock> hostCached = allocateMemblocks(m_Device->device(), heap, _256MB, type.second, downloadbuffer ? 1 : 2);
            if(hostCached.size() > 0)
            {
                Logger::log("Allocated " +  std::to_string(hostCached.size()) + " 256MB HostLocalHostCached buffer for uploads");
                moveAppend(hostCached, m_HostLocalCachedMemoryBlocks);
                break;
            }
        }
    }
    else if(hostCoherentTypes.size() > 0)
    {
        //Else host coherent...
        for(const auto& type : hostCoherentTypes)
        {
            vector<MemoryBlock> hostCoherent = allocateMemblocks(m_Device->device(), heap, _256MB, type.second, downloadbuffer ? 1 : 2);
            if(hostCoherent.size() > 0)
            {
                Logger::log("Allocated " + std::to_string(hostCoherent.size()) + " 256MB HostLocalHostCoherent buffer for uploads");
                moveAppend(hostCoherent, m_HostLocalHostCoherentMemoryBlocks);
                break;
            }
        }

    }
}

