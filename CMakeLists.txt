cmake_minimum_required(VERSION 3.11)
project(smol)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB SRC "src/*.h" "src/*.c")
add_executable(${PROJECT_NAME} ${SRC})