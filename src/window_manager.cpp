#include "window_manager.hpp"
#include <glog/logging.h>

#include "utils.hpp"
#include "EventHandlers.hpp"

bool XTau::WindowManager::_wm_detected = false;

std::unique_ptr<XTau::WindowManager> XTau::WindowManager::Create(){
    Display* display = XOpenDisplay(nullptr);
    if(display == nullptr){
        LOG(ERROR) << "Failed to open X display" << XDisplayName(nullptr);
        return nullptr;
    }

    return std::unique_ptr<XTau::WindowManager>(new WindowManager(display));
}

XTau::WindowManager::WindowManager(Display* display) : _display(CHECK_NOTNULL(display)), _root(DefaultRootWindow(_display)){

}

XTau::WindowManager::~WindowManager(){
    XCloseDisplay(_display);
}

void XTau::WindowManager::Run(){
    // 1. Initialization
    // a. Select events on root window. Use a special error handler so we can exit gracefully if another wm is already running
    _wm_detected = false;
    XSetErrorHandler(&XTau::WindowManager::OnWMDetected);
    XSelectInput(this->_display, this->_root, SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(this->_display, false);
    if(_wm_detected){
        LOG(ERROR) << "Detected another window manager on display" << XDisplayString(this->_display);
        return;
    }

    // b. Set error handler
    XSetErrorHandler(&XTau::WindowManager::OnXError);

    // c. Grab XServer to prevent windows from changing under us while we frame them
    XGrabServer(this->_display);
    
    // d. Frame existing top level windows
    //  i. Query existing top-level windows
    Window returned_root, returned_parent;
    Window* top_level_windows;
    unsigned int num_top_level_windows;
    CHECK(XQueryTree(
        this->_display,
        this->_root,
        &returned_root,
        &returned_parent,
        &top_level_windows,
        &num_top_level_windows
    ));

    //  ii. Frame each top level window
    for(unsigned int i = 0; i < num_top_level_windows; i++){
        Frame(top_level_windows[i], true /* was_created_before_window_manager*/);
    }
    //  iii. Free top-level window array
    XFree(top_level_windows);
    // e. Ungrab XServer
    XUngrabServer(this->_display);

    // 2. Main event loop
    while(true){
        XEvent e;
        XNextEvent(this->_display, &e);
        LOG(INFO) << "Received event: " << ToString(e);

        switch(e.type){
            case CreateNotify:
                XTau::EventHandlers::OnCreateNotify(e.xcreatewindow);
                break;
            case ConfigureRequest:
                OnConfigureRequest(e.xconfigurerequest);
                break;
            case MapRequest:
                OnMapRequest(e.xmaprequest);
                break;
            case UnmapNotify:
                OnUnmapNotify(e.xunmap);
                break;
            default:
                LOG(WARNING) << "Ignored event";
        }
    }
}

int XTau::WindowManager::OnWMDetected(Display* display, XErrorEvent* e){
    // In the case of an already running window manager, the error code from XSelectInput is BadAccess. We don't expect this handler to receive any other errors.
    CHECK_EQ(static_cast<int>(e->error_code), BadAccess);
    _wm_detected = true;
    return 0;
}

int XTau::WindowManager::OnXError(Display* display, XErrorEvent* e){
    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
    LOG(ERROR)  << "Received X error:\n"
                << "    Request: " << int(e->request_code)
                << " - " << XTau::XRequestCodeToString(e->request_code) << "\n"
                << "    Error code: " << int(e->error_code)
                << " - " << error_text << "\n"
                << "    Resource ID: " << e->resourceid;
    return 0;
}