#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名字
project(OpenGL_Lecture)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#往项目中加入一个全局的预编译宏(CMakeLists中使用全局预编译宏来控制是否开启错误检查)
add_definitions (-DDEBUG)

#规定头文件搜索路径
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)

link_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib)

#添加wrapper文件
add_subdirectory(wrapper)

#添加application文件
add_subdirectory(application)


#本工程所有cpp文件编译链接，生成exe
add_executable(openglStudy "main.cpp" "glad.c")

target_link_libraries(openglStudy glfw3.lib wrapper app)