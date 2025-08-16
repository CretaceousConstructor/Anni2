cmake_minimum_required(VERSION 3.30)
find_package(Vulkan REQUIRED)
find_package(spdlog CONFIG REQUIRED)


# if (MSVC)
set(BUILD_SHARED_LIBS FALSE)
add_compile_options(/std:c++latest)
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
# endif()

project(AnniBase)
add_definitions(-DNOMINMAX)
add_definitions(-DKHRONOS_STATIC)

set(ProjectType STATIC)


file(GLOB_RECURSE FILE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)


add_library(AnniBase ${ProjectType} ${FILE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${FILE_SOURCES})

target_include_directories(AnniBase PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include") 
target_include_directories(AnniBase INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include") 

target_link_libraries(AnniBase PUBLIC Vulkan::Vulkan) 
target_link_libraries(AnniBase PUBLIC spdlog::spdlog)
target_link_libraries(AnniBase PUBLIC glfw)

# target_link_libraries(AnniBase PRIVATE Vulkan::Vulkan AnniGLTF AnniImGui glm vma stb_image dxc)
# target_link_libraries(AnniBase PRIVATE glfw)
# target_link_libraries(AnniBase PRIVATE ktx)
