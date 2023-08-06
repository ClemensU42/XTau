#pragma once

#include <X11/Xlib.h>

namespace XTau::EventHandlers{
    void OnCreateNotify(const XCreateWindowEvent& e);
    
}