project (ls)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

message(STATUS ${PROJECT_BINARY_DIR})

set(LS_SRCS
    ${PROJECT_SOURCE_DIR}/ls.c
)

include_directories("${PROJECT_BINARY_DIR}")
add_executable (runls ${LS_SRCS})
include_directories("${PROJECT_INCLUDE_DIR}")
