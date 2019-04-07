cmake_minimum_required(VERSION 3.1.0)
project(SherkSQL C)

set(CMAKE_C_STANDARD 99)

# 使用此配置头文件，头文件不用再写成 "../../../" 这样丑陋的路径了
include_directories(../src)

aux_source_directory(. source_list)

add_executable(SherkSQL ${source_list})

