cmake_minimum_required(VERSION 3.15) # 根据你的需求进行修改
project(sample C)

set(CMAKE_C_STANDARD 11) # 根据你的C编译器支持情况进行修改
set(CMAKE_EXECUTABLE_SUFFIX ".html") # 编译生成.html



add_compile_options(-fno-stack-protector)
include_directories(vendor) # 使得我们能引用第三方库的头文件
option(ENABLE_CJSON_TEST OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(vendor/cJSON)

add_executable(sample main.c)

# 设置Emscripten的编译链接参数，我们等等会讲到一些常用参数

# set_target_properties(sample PROPERTIES LINK_FLAGS "-s EXIT_RUNTIME=1")
# set_target_properties(sample PROPERTIES LINK_FLAGS " \
#     -s WASM=0 \
#     -s TOTAL_MEMORY=16777216 \
#     -s EXIT_RUNTIME=1 \
#     -s EXPORTED_FUNCTIONS=\"['_json_parse']\" \
# ")

set_target_properties(sample PROPERTIES LINK_FLAGS "\
    -s NO_EXIT_RUNTIME=1 \
    -s FETCH=1 \
")

target_link_libraries(sample cjson) # 将第三方库与主程序进行链接
