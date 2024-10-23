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

# message(CMAKE_LIBRARY_OUTPUT_DIRECTORY_<CONFIG>="${CMAKE_LIBRARY_OUTPUT_DIRECTORY_<CONFIG>}")

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Resources
)
add_dependencies(ShapesSandbox_Testing copy_assets)

include(./vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(glad CONFIG REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE OpenGL::GL)

find_package(Stb REQUIRED)
target_include_directories(ShapesSandbox_Testing PRIVATE ${Stb_INCLUDE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(ShapesSandbox_Testing PRIVATE glm::glm)