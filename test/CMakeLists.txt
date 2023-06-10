cmake_minimum_required(VERSION 3.0)
project(TEST)
set(CMAKE_CXX_FLAGS   "-g")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# 指定源文件或者动态库对应的头文件路径
include_directories(${PROJECT_SOURCE_DIR}/../include)
# 指定要链接的动态库的路径
link_directories(${PROJECT_SOURCE_DIR}/../lib)
# 添加并生成一个可执行程序
add_executable(test01 ${SRC_LIST})
# 指定要链接的动态库
target_link_libraries(test01 json)

