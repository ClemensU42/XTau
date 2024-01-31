#include "window_manager.hpp"
#include <glog/logging.h>

using ::std::unique_ptr;

xcb_screen_t *screen_of_display (xcb_connection_t *c, int screen)
{
    xcb_screen_iterator_t iter;

    iter = xcb_setup_roots_iterator (xcb_get_setup (c));
    for (; iter.rem; --screen, xcb_screen_next (&iter))
        if (screen == 0)
        return iter.data;

    return NULL;
}

unique_ptr<WindowManager> WindowManager::Create(){
    xcb_connection_t* connection = xcb_connect(NULL, NULL);
    if(xcb_connection_has_error(connection)){
        LOG(ERROR) << "Failed to open X display";
        return nullptr;
    }

    return unique_ptr<WindowManager>(new WindowManager(connection));
}

WindowManager::WindowManager(xcb_connection_t* connection) : connection(CHECK_NOTNULL(connection)){
    xcb_screen_t* screen;
    screen = screen_of_display(connection, 0);
    if(screen) root = screen->root;
}

WindowManager::~WindowManager(){
    xcb_disconnect(this->connection);
}

void WindowManager::Run(){
    //TODO
}