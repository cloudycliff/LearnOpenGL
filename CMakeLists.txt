cmake_minimum_required(VERSION 3.7)

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk)

project(GinOpenGL)

set(CMAKE_CXX_STANDARD 11)

set(MAIN_FILE src/window.cpp)

set(SOURCE_FILES ${MAIN_FILE} GLAD/src/glad.c lib/gin.cpp)
set(HEADER_FILES include/shader.h include/vapp.h include/vgl.h include/camera.h include/texture.h)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/build)

include_directories(include)

include_directories(GLAD/include)

#Include library files
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

#COPY seems not working everytime...use symbolic link instead...
#file(COPY resources DESTINATION ${dir}/build)
#file(COPY shaders DESTINATION ${dir}/build)

add_executable(GinOpenGL ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(GinOpenGL glfw3 "-framework Cocoa" "-framework CoreVideo" "-framework IOKit" "-framework OpenGL")
