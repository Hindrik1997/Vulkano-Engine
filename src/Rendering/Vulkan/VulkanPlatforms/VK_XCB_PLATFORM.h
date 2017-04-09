//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_VK_XCB_PLATFORM_H
#define VULKANOENGINE_VK_XCB_PLATFORM_H


#include <xcb/xcb.h>
#include <vulkan/vulkan.h>

#define VK_PLATFORM VK_XCB_PLATFORM

class VK_XCB_PLATFORM{
private:
    xcb_connection_t* m_connection = nullptr;
    xcb_window_t m_window;
    xcb_atom_t m_wmDeleteWin;
    xcb_atom_t m_wmProtocols;
    xcb_screen_t * m_screen;
public:
    VK_XCB_PLATFORM();
    ~VK_XCB_PLATFORM();

    xcb_connection_t* getConnection();
    xcb_window_t getWindow();
    VkResult CreateSurface(VkInstance instance, VkSurfaceKHR surface);



    bool processAPI(float deltaTime);

};

VkResult CreateXCBSurfaceKHR(VkInstance instance, VkXcbSurfaceCreateInfoKHR info, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR& surface);

#endif //VULKANOENGINE_VK_XCB_PLATFORM_H
