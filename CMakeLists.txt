set(VCPKG_FEATURE_FLAGS "manifests")

cmake_minimum_required(VERSION 3.5.0)
project(ShapesSandbox_Testing VERSION 0.1.0 LANGUAGES C CXX)

add_executable(ShapesSandbox_Testing main.cpp)

set(CMAKE_MAIN_PROJECT ${PROJECT_NAME} CACHE INTERNAL "")

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




################################
# Compile Statically for Windows
################################
if(WIN32) # Need to compile statically to prevent Windows .dll not found error for glfw
set(VCPKG_HOST_TRIPLET x64-windows-static)
set(VCPKG_TARGET_TRIPLET x64-windows-static)
elseif(APPLE)
set(VCPKG_HOST_TRIPLET arm64-osx)
set(VCPKG_TARGET_TRIPLET arm64-osx)
elseif(UNIX AND NOT APPLE)
set(VCPKG_HOST_TRIPLET x64-linux)
set(VCPKG_TARGET_TRIPLET x64-linux)
endif()

###############
# Generate Docs
###############
OPTION(BUILD_WITH_DOCS "Generate Docs" ON)
set(BUILD_WITH_DOCS OFF) # Set this to ON to manually build with docs.
IF(BUILD_WITH_DOCS)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/docs)
ENDIF()

#############################################################
# Add Resources to build directory so program can access them
#############################################################
add_custom_target(copy_assets
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Resources
)
add_dependencies(ShapesSandbox_Testing copy_assets)

#############################################################
# Include VCPKG so CMake knows how to find 3rd party packages
#############################################################
include(./vcpkg/scripts/buildsystems/vcpkg.cmake)

########################
# Find and link packages
########################
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
