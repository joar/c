project (gfx)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

message(STATUS ${PROJECT_BINARY_DIR})

set(GFX_SRCS
    ${PROJECT_SOURCE_DIR}/gfx.c
)

set(GFX_LIBS
    ${PROJECT_SOURCE_DIR}/shader_utils.c
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(rungfx ${GFX_SRCS})
add_library(shader_utils ${GFX_LIBS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(rungfx
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    shader_utils)

include_directories("${PROJECT_INCLUDE_DIR}")
