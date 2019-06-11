#[[
Equivalent to:

gcc -g -O3 -m64 -std=c99 -flto -Wall -Wextra -Wpedantic \
    pyrepl.c -o pyrepl \
    $(python3-config --includes) \
    $(python3-config --ldflags)
]]

project(pyrepl)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-m64 -std=c99 -flto -Wall -Wextra -Wpedantic -Xlinker -export-dynamic)

# find_package(Python3 COMPONENTS Interpreter Development) results in Python.h not being
# found so in the meantime I will use this janky bullshit that has the nice property of
# working on my machine instead

include_directories(/usr/include/python3.7m)
link_directories(/usr/lib)
link_libraries(python3.7m crypt pthread dl util m)

add_executable(
    pyrepl
    pyrepl.c
)
