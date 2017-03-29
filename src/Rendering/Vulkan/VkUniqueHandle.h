//
// Created by hindrik on 29-3-17.
//

#ifndef VULKANOENGINE_VKUNIQUEHANDLE_H
#define VULKANOENGINE_VKUNIQUEHANDLE_H

#include "VulkanPlatforms/VulkanPlatform.h"
#include "../../Utility Classes/NonCopyable.h"
#include <functional>

using std::function;

template <typename T>
class VkUniqueHandle : NonCopyable
{
private:
    T m_Object {VK_NULL_HANDLE};
    std::function<void(T)> m_Deleter;
public:
    VkUniqueHandle() : VkUniqueHandle([](T, VkAllocationCallbacks*) {}) {}

    VkUniqueHandle(std::function<void(T, VkAllocationCallbacks*)> deletef)
    {
        this->m_Deleter = [=](T obj) { deletef(obj, nullptr); };
    }

    VkUniqueHandle(const VkUniqueHandle<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
    {
        this->m_Deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
    }

    VkUniqueHandle(const VkUniqueHandle<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
    {
        this->m_Deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
    }

    ~VkUniqueHandle() {
        cleanup();
    }

    const T* operator &() const {
        return &m_Object;
    }

    T get()
    {
        return m_Object;
    }

    std::function<void(T)> get_deleter()
    {
        return m_Deleter;
    }

    T release()
    {
        T temp = m_Object;
        m_Object = VK_NULL_HANDLE;
        return temp;
    }

    void reset(T handle)
    {
        if(m_Object != VK_NULL_HANDLE)
        {
            cleanup();
        }
        m_Object = handle;
    }

    void swap(VkUniqueHandle<T>& other)
    {
        T oldHandle = m_Object;
        m_Object = other.release();
        other.reset(oldHandle);

    }

    operator T() const {
        return m_Object;
    }

    void operator=(T rhs) {
        if (rhs != m_Object) {
            cleanup();
            m_Object = rhs;
        }
    }

    template<typename V>
    bool operator==(V rhs) {
        return m_Object == T(rhs);
    }

    operator bool() const
    {
        return m_Object == VK_NULL_HANDLE;
    }

private:
    void cleanup() {
        if (m_Object != VK_NULL_HANDLE) {
            m_Deleter(m_Object);
        }
        m_Object = VK_NULL_HANDLE;
    }
};








#endif //VULKANOENGINE_VKUNIQUEHANDLE_H