cmake_minimum_required (VERSION 3.0.2)

project (Core)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

aux_source_directory(core/src/ SOURCE)
add_executable(Core ${SOURCE})
include_directories(core/src)

#OPENGL (GL and GLU)
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(Core ${OPENGL_LIBRARIES})
endif()

#GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(Core ${GLEW_LIBRARIES})
endif()

#SDL2
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(Core ${SDL2_LIBRARY})
endif()

#SDL_image
find_package(SDL2_image REQUIRED)
if(SDL2IMAGE_FOUND)
    include_directories(${SDL2IMAGE_INCLUDE_DIR})
    target_link_libraries(Core ${SDL2IMAGE_LIBRARY})
endif()

#gdxpp
add_library(gdxpp SHARED IMPORTED )
set_target_properties(gdxpp PROPERTIES IMPORTED_LOCATION ../android/dependencies/jni/gdxpp/build/libgdxpp.so)
include_directories(android/dependencies/jni/gdxpp/src/)

#final linkage
target_link_libraries(Core gdxpp ${GLEW_LIBRARIES})

#WIP: pull down libgdxpp git repo, and include it in project
#ExternalProject_Add(gdxpp GIT_REPOSITORY https://github.com/ccour004/libgdxpp.git
#	BINARY_DIR build BUILD_COMMAND cmake .)
#ExternalProject_Get_Property(gdxpp install_dir)
#include_directories(${install_dir})
#add_dependencies(Core gdxpp)
#target_link_libraries(Core ${install_dir}/build/libgdxpp.so)


