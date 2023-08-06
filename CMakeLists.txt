cmake_minimum_required(VERSION 3.24)

project(XTau VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

find_package(X11 REQUIRED)
find_package(glog REQUIRED)


add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} glog::glog ${X11_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR} include)