project (gfx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++0x")

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

message(STATUS ${PROJECT_BINARY_DIR})

set(GFX_SRCS
    ${PROJECT_SOURCE_DIR}/gfx.cpp
    ${PROJECT_SOURCE_DIR}/shader_utils.cpp
    ${PROJECT_SOURCE_DIR}/res_texture.c
)

set(GFX_LIBS
    ${PROJECT_SOURCE_DIR}/shader_utils.cpp
)

set(_FILES
    triangle.v.glsl
    triangle.f.glsl
)

install(FILES ${_FILES} DESTINATION bin)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(rungfx ${GFX_SRCS})
#add_library(shader_utils ${GFX_LIBS})
#add_library(res_texture ${PROJECT_SOURCE_DIR}/res_texture.c)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(rungfx
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    m
)

include_directories("${PROJECT_INCLUDE_DIR}")
