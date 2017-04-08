//
// Created by hindrik on 8-4-17.
//

#ifndef VULKANOENGINE_WINDOWHANDLEWRAPPER_H
#define VULKANOENGINE_WINDOWHANDLEWRAPPER_H

#include "VulkanPlatform.h"

class WindowHandleWrapper
{
private:
    WindowHandle m_Handle = 65535;
    VK_PLATFORM& m_Platform;
public:
    WindowHandleWrapper(VK_PLATFORM& platform) : m_Handle(65535), m_Platform(platform){}
    WindowHandleWrapper(WindowHandle handle, VK_PLATFORM& platform) : m_Handle(handle), m_Platform(platform){}
    ~WindowHandleWrapper(){
        if(m_Handle != 65535)
            m_Platform.destroyWindow(m_Handle);
    }

    WindowHandleWrapper(const WindowHandleWrapper&) = delete;
    WindowHandleWrapper(WindowHandleWrapper&& rhs) : m_Platform(rhs.m_Platform)
    {
        if(m_Handle != 65535)
            m_Platform.destroyWindow(m_Handle);
        m_Handle = rhs.m_Handle;
        rhs.m_Handle = 65535;
    };
    WindowHandleWrapper& operator=(WindowHandleWrapper&&) = delete;
    WindowHandleWrapper& operator=(const WindowHandleWrapper&) = delete;

    operator WindowHandle() const { return m_Handle; }

    WindowHandle swap(WindowHandle handle) { WindowHandle t = m_Handle; m_Handle = handle; return t; }
    WindowHandle release() { WindowHandle t = m_Handle; m_Handle = 65535; return t;}
    void reset(WindowHandle handle) {
        if(m_Handle != 65535)
            m_Platform.destroyWindow(m_Handle);
        m_Handle = handle;
    }
};

#endif //VULKANOENGINE_WINDOWHANDLEWRAPPER_H
