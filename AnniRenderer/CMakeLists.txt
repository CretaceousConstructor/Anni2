cmake_minimum_required(VERSION 3.30)

#if (MSVC)
add_compile_options(/std:c++latest)
set(BUILD_SHARED_LIBS FALSE)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#endif()

project(AnniRenderer)
add_definitions(-DNOMINMAX)
add_definitions(-DKHRONOS_STATIC)


file(GLOB_RECURSE FILE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "FILE_SOURCES : ${FILE_SOURCES}")

add_executable(AnniRenderer  ${FILE_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${FILE_SOURCES})


target_include_directories(AnniRenderer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/AnniRenderer/include") 

#target_link_libraries(AnniRenderer PRIVATE AnniGLTF AnniBase AnniImGui Vulkan::Vulkan glfw ktx stb_image dxc)
target_link_libraries(AnniRenderer PRIVATE AnniBase)

