cmake_minimum_required(VERSION 3.10)

# 定义静态库
add_library(ini STATIC ini.c)

# 添加头文件搜索路径
target_include_directories(ini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
