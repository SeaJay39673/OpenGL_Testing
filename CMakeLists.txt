set(VCPKG_FEATURE_FLAGS "manifests")

cmake_minimum_required(VERSION 3.5.0)
project(ShapesSandbox_Testing VERSION 0.1.0 LANGUAGES C CXX)

add_executable(ShapesSandbox_Testing main.cpp)


include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(WIN32) # Need to compile statically to prevent Windows .dll not found error for glfw
    set(VCPKG_HOST_TRIPLET x64-windows-static)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)
endif()

include(./vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(glad CONFIG REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE OpenGL::GL)