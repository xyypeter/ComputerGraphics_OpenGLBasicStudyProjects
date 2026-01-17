#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#往项目中加入一个全局的预编译宏(CMakeLists中使用全局预编译宏来控制是否开启错误检查)
add_definitions (-DDEBUG)

#本工程的名字
project(OpenGL_Lecture)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#拷入资源文件
#file(GLOB ASSETS "./assets" )
#file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

#规定头文件搜索路径
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)

link_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib)

#添加wrapper文件
add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)

#本工程所有cpp文件编译链接，生成exe
add_executable(openglStudy "main.cpp" "glad.c")

target_link_libraries(openglStudy glfw3.lib wrapper app fw)

# ========= 资源管理（关键） =========
# 拷贝 assets 到运行目录
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_BINARY_DIR}
)

# 注入资源根目录给 C++
set(ASSETS_RUNTIME_DIR "${CMAKE_BINARY_DIR}/assets")

target_compile_definitions(openglStudy PRIVATE
    DEBUG
    ASSETS_DIR="${ASSETS_RUNTIME_DIR}"
)