cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

file(GLOB sources "*.cpp" "*.h")

project(memory)


add_executable(${PROJECT_NAME} ${sources})


target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
