#include "EventHandlers.hpp"

#include <X11/Xlib.h>
#include <glog/logging.h>

#include "utils.hpp"
#include "window_manager.hpp"

void XTau::WindowManager::OnConfigureRequest(const XConfigureRequestEvent& e){
    XWindowChanges changes;

    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;

    if(_clients.count(e.window)){
        const Window frame = _clients[e.window];
        XConfigureWindow(this->_display, frame, e.value_mask, &changes);
        LOG(INFO) << "Resize [" << frame << "] to " << XTau::Size<int>(e.width, e.height);
    }

    XConfigureWindow(this->_display, e.window, e.value_mask, &changes);
    LOG(INFO) << "Resize " << e.window << " to " << XTau::Size<int>(e.width, e.height);
}