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
    T m_Object                  = VK_NULL_HANDLE;
    function<void(T)> m_Deleter = nullptr;
public:
    VkUniqueHandle() = default;

    VkUniqueHandle(function<void(T,VkAllocationCallbacks*)> deletef)
    {
        m_Deleter = [deletef](T obj){ deletef(obj, nullptr); };
    }

    VkUniqueHandle(VkInstance instance, function<void(VkInstance,T,VkAllocationCallbacks*)> deletef)
    {
        m_Deleter = [instance,deletef](T obj){ deletef(instance,obj, nullptr); };
    }

    VkUniqueHandle(VkDevice device, function<void(VkDevice,T,VkAllocationCallbacks*)> deletef)
    {
        m_Deleter = [device,deletef](T obj){ deletef(device,obj, nullptr); };
    }

    VkUniqueHandle(T value, VkInstance instance, function<void(VkInstance,T,VkAllocationCallbacks*)> deletef)
    {
        m_Deleter = [instance,deletef](T obj){ deletef(instance,obj, nullptr); };
        m_Object = value;
    }

    VkUniqueHandle(T value, VkDevice device, function<void(VkDevice,T,VkAllocationCallbacks*)> deletef)
    {
        m_Deleter = [device,deletef](T obj){ deletef(device,obj, nullptr); };
        m_Object = value;
    }

    VkUniqueHandle(VkUniqueHandle&& other) noexcept
    {
        cleanup();
        m_Object = std::move(other.m_Object);
        m_Deleter = std::move(other.m_Deleter);
        other.m_Deleter = nullptr;
        other.m_Object = VK_NULL_HANDLE;
    }

    VkUniqueHandle& operator=(VkUniqueHandle&& other) noexcept
    {
        cleanup();
        m_Object = std::move(other.m_Object);
        m_Deleter = std::move(other.m_Deleter);
        other.m_Deleter = nullptr;
        other.m_Object = VK_NULL_HANDLE;
        return *this;
    };

    VkUniqueHandle(const VkUniqueHandle&) = delete;
    VkUniqueHandle& operator=(const VkUniqueHandle&) = delete;



    ~VkUniqueHandle() {
        cleanup();
    }

    const T* operator &() const {
        return &m_Object;
    }

    T get() const
    {
        return m_Object;
    }

    std::function<void(T)> get_deleter() const
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
        cleanup();
        m_Object = handle;
    }

    T* reset()
    {
        cleanup();
        return &m_Object;
    }

    void swap(VkUniqueHandle& other)
    {
        T oldHandle = m_Object;
        function<void(T)> f = m_Deleter;
        m_Object = other.release();
        m_Deleter = other.m_Deleter;
        other.reset(oldHandle);
        other.m_Deleter = f;
    }

    operator T() const
    {
        return m_Object;
    }

    void operator=(T rhs)
    {
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