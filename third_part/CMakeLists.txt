cmake_minimum_required(VERSION 3.15)
find_package(Vulkan REQUIRED) #if not, use header and .lib(static) way of importing
#if (MSVC)
    #add_compile_options(/std:c++latest)
    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    #set(BUILD_SHARED_LIBS FALSE)
#endif()
add_compile_options(/std:c++latest)
set(BUILD_SHARED_LIBS FALSE)
#add_definitions(-DKHRONOS_STATIC)

#HEADER ONLY LIBRARYS
#vma, glm, stb_image are header only libs so we only need the including path
#add_library(glm INTERFACE)
#add_library(vma INTERFACE)
#add_library(stb_image INTERFACE)

#target_include_directories(glm INTERFACE glm)
#target_include_directories(vma INTERFACE vma)
#target_include_directories(stb_image INTERFACE stb_image)

#GLFW IS A LIBRARY WITH ONLY .LIB AND .H FILES 没有source file的只能用INTERFACE
 #${CMAKE_CURRENT_SOURCE_DIR}
add_library(glfw INTERFACE)
target_include_directories(glfw INTERFACE glfw/include)
target_link_directories(glfw INTERFACE glfw/lib) 
target_link_libraries(glfw INTERFACE glfw3.lib) 

#DXC IS A LIBRARY WITH ONLY .LIB AND .H FILES 没有source file的只能用INTERFACE
#add_library(dxc INTERFACE)
#target_include_directories(dxc INTERFACE dxc/include)
#target_link_directories(dxc INTERFACE dxc/lib) 
#target_link_libraries(dxc INTERFACE dxcompiler.lib) 

#KTX IS A lIB WITH ONLY .LIB AND .H FILES 没有source file的只能用INTERFACE
#add_library(ktx INTERFACE)
#target_include_directories(ktx INTERFACE ktx/include)
#target_link_directories(ktx INTERFACE ktx/lib) 
#target_link_libraries(ktx INTERFACE ktx.lib) 

#COMPILE FROM SOURCE CODE, AND SUB-DIR which ALSO HAS Cmakelists.txt FILE
#add_subdirectory(fastgltf)
#add_library(AnniGLTF INTERFACE)
#target_link_libraries(AnniGLTF INTERFACE fastgltf)

#COMPILE FROM SOURCE CODE, AND NOT IN SUB-DIR WAY.
#file(GLOB ImguiSourceFiles "imgui/*.cpp")
#file(GLOB ImguiHeaderFiles "imgui/*.h")
#
#add_library(AnniImGui STATIC
#        ${ImguiSourceFiles}
#        ${ImguiHeaderFiles}
#)

#target_include_directories(AnniImGui PUBLIC imgui)
#target_link_libraries(AnniImGui PRIVATE glfw)
#target_link_libraries(AnniImGui PRIVATE Vulkan::Vulkan)
#**************************************************
