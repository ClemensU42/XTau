extern "C" {
#include <xcb/xcb.h>
}

#include <memory>

class WindowManager{
public:
    static ::std::unique_ptr<WindowManager> Create();
    ~WindowManager();
    void Run();

private:
    WindowManager(xcb_connection_t* connection);

    xcb_connection_t* connection;
    xcb_window_t root;
};
