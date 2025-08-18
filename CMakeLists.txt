cmake_minimum_required(VERSION 3.30)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "Vcpkg toolchain file")

project(Anni2 VERSION 1.0.0)
add_definitions(-DNOMINMAX)


#if (MSVC)
#    add_compile_options(/std:c++latest)
#    set(BUILD_SHARED_LIBS FALSE)
#    #set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#endif()

add_compile_options(/std:c++latest)
set(BUILD_SHARED_LIBS FALSE)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")



set (SubProjects
    third_party
    AnniBase
    AnniRenderer
)

#foreach(Project IN LISTS SubProjects)
#    if(NOT TARGET Project)  #header guard version
#       add_subdirectory(${Project})
#    endif()
#endforeach()

add_subdirectory(third_part)
add_subdirectory(AnniBase)
add_subdirectory(AnniRenderer)

#set_property(TARGET AnniRenderer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}") #为了方便debug时，文件路径能正确