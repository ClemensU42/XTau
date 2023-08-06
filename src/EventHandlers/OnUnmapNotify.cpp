#include "window_manager.hpp"

#include <X11/Xlib.h>
#include <glog/logging.h>

void XTau::WindowManager::OnUnmapNotify(const XUnmapEvent& e){
    // If the window is a client window we manage, unframe it upon UnmapNotify. We
    // need the check because we will receive an UnmapNotify event for a frame
    // window we just destroyed ourselves.
    if(!_clients.count(e.window)){
        LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
        return;
    }

    // Ignore event if it is triggered by reparenting a window that was mapped
    // before the window manager started.
    //
    // Since we receive UnmapNotify events from the SubstructureNotify mask, the
    // event attribute specifies the parent window of the window that was
    // unmapped. This means that an UnmapNotify event from a normal client window
    // should have this attribute set to a frame window we maintain. Only an
    // UnmapNotify event triggered by reparenting a pre-existing window will have
    // this attribute set to the root window.
    if(e.event == this->_root){
        LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window " << e.window;
        return;
    }

    Unframe(e.window);
}

void XTau::WindowManager::Unframe(Window w){
    const Window frame = this->_clients[w];

    XUnmapWindow(this->_display, frame);

    XReparentWindow(this->_display, w, this->_root, 0, 0);

    XRemoveFromSaveSet(this->_display, frame);

    this->_clients.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}