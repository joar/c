cmake_minimum_required (VERSION 2.6)

project (ls)

add_definitions ("-std=c99")

add_executable (ls ls.c)

install (TARGETS ls DESTINATION bin)
