//
// Created by hindrik on 30-1-17.
//

#ifndef VULKANOENGINE_VK_XCB_PLATFORM_H
#define VULKANOENGINE_VK_XCB_PLATFORM_H


#include <xcb/xcb.h>

class VulkanPlatform{
private:
    xcb_connection_t* m_connection = nullptr;
    xcb_window_t m_window;
    xcb_atom_t m_wmDeleteWin;
    xcb_atom_t m_wmProtocols;
public:
    VulkanPlatform();
    ~VulkanPlatform();

    xcb_connection_t* getConnection();
    xcb_window_t getWindow();

    bool processAPI(float deltaTime);

};


#endif //VULKANOENGINE_VK_XCB_PLATFORM_H
