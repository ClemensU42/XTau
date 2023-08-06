#include "window_manager.hpp"

#include <glog/logging.h>

void XTau::WindowManager::OnMapRequest(const XMapRequestEvent& e){
    Frame(e.window, false);
    XMapWindow(this->_display, e.window);
}

void XTau::WindowManager::Frame(Window w, bool was_created_before_window_manager){
    const unsigned int BORDER_WIDTH = 3;
    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR = 0x0000ff;

    // 1. Retrieve attributes of window to frame
    XWindowAttributes x_window_attrs;
    CHECK(XGetWindowAttributes(this->_display, w, &x_window_attrs));

    // 2. see Framing existing top-level windows section below
    if(was_created_before_window_manager)
        if(x_window_attrs.override_redirect || x_window_attrs.map_state != IsViewable)
            return;

    // 3. Create frame
    const Window frame = XCreateSimpleWindow(
        this->_display,
        this->_root,
        x_window_attrs.x,
        x_window_attrs.y,
        x_window_attrs.width,
        x_window_attrs.height,
        BORDER_WIDTH,
        BORDER_COLOR,
        BG_COLOR
    );

    // 3. Select events on frame
    XSelectInput(this->_display, frame, SubstructureRedirectMask | SubstructureNotifyMask);

    // 4. Add client to save set, so taht it will be restored and kept alive if we crash
    XAddToSaveSet(this->_display, w);
    // 5. reparent client window
    XReparentWindow(this->_display, w, frame, 
    0, 0); // offset of the client window within frame

    // 6. Map frame
    XMapWindow(this->_display, frame);

    // 7. Save frame handle
    this->_clients[w] = frame;

    //TODO: 8. Grab events for window management actions on client window

    LOG(INFO) << "Framed Window " << w << " [" << frame << "]";
}