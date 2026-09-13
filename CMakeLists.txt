cmake_minimum_required(VERSION 3.10)
project(OTAVauleControl C)

set(CMAKE_C_STANDARD 11)
set(SRC_FILES
    main.c
    netinter.c
    lorahal.c
    ipc.c
    comm.c
    valve.c
)

add_executable(otavalve ${SRC_FILES})

# include current directory for headers
target_include_directories(otavalve PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable warnings
if(MSVC)
    target_compile_options(otavalve PRIVATE /W4)
else()
    target_compile_options(otavalve PRIVATE -Wall -Wextra -Wpedantic)
endif()
