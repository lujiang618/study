cmake_minimum_required(VERSION 3.10.0)


project(run)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin CACHE STRING "Output directory")

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME} WIN32 main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
     # 按依赖关系正确排序
    ${CMAKE_SOURCE_DIR}/lib/libavfilter.a
    ${CMAKE_SOURCE_DIR}/lib/libavformat.a
    ${CMAKE_SOURCE_DIR}/lib/libavcodec.a
    ${CMAKE_SOURCE_DIR}/lib/libswscale.a
    ${CMAKE_SOURCE_DIR}/lib/libswresample.a
    ${CMAKE_SOURCE_DIR}/lib/libpostproc.a
    ${CMAKE_SOURCE_DIR}/lib/libavutil.a

     # 系统库
    Threads::Threads
    ${ZLIB_LIBRARIES}
    dl
    m
    z
    bz2
    lzma

    # 视频处理库
    x264
    va
    va-drm
    va-x11
    drm
    X11
)

