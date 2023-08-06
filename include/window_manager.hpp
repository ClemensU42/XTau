#pragma once

extern "C" {
    #include <X11/Xlib.h>
}
#include <memory>
#include <unordered_map>

namespace XTau{
    class WindowManager{
    public:
        /// @brief Factory method for establishing a connection to an XServer and creating a WindowManager Instance
        /// @return A unique_ptr to the created WindowManager
        static std::unique_ptr<WindowManager> Create();

        /// @brief Disconnects from the XServer
        ~WindowManager();

        /// @brief The entry point to the WindowManager class. Enters the main event loop
        void Run();

        void OnConfigureRequest(const XConfigureRequestEvent& e);
        void OnMapRequest(const XMapRequestEvent& e);
        void OnUnmapNotify(const XUnmapEvent& e);

        void Unframe(Window w);
        void Frame(Window w, bool was_created_before_window_manager);

    private:
        static int OnXError(Display* display, XErrorEvent* e);

        static int OnWMDetected(Display* display, XErrorEvent* e);

        static bool _wm_detected;

        WindowManager(Display* display);

        Display* _display;
        std::unordered_map<Window, Window> _clients;
        const Window _root;
    };
}