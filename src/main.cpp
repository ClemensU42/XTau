
#include <cstdlib>

#include <glog/logging.h>

#include "window_manager.hpp"

int main(int argc, char** argv){
    google::InitGoogleLogging(argv[0]);

    std::unique_ptr<XTau::WindowManager> window_manager(XTau::WindowManager::Create());
    if(!window_manager){
        LOG(ERROR) << "Failed to initialize window manager!";
        return EXIT_FAILURE;
    }

    window_manager->Run();

    return EXIT_SUCCESS;
}