project(jnp_task_7)
cmake_minimum_required(VERSION 3.9)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -pedantic -I.. -MD")

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${CMAKE_THREAD_LIBS_INIT} )
    endif()
endmacro()

add_executable(42 42.cc)
