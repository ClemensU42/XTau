cmake_minimum_required(VERSION 3.25)

project(XTau VERSION 0.0.1)

find_package(X11 REQUIRED)
find_package(glog REQUIRED)

set(EXEC_NAME "XTau")

add_executable(${EXEC_NAME} src/main.cpp)

add_subdirectory(src)

if(X11_FOUND)
    target_include_directories(${EXEC_NAME} PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(${EXEC_NAME} ${X11_LIBRARIES} xcb glog::glog)
else()
    message(FATAL_ERROR "X11 library has not been found!")
endif()

target_include_directories(${EXEC_NAME} PRIVATE include/)

