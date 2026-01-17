#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)
add_definitions (-DDEBUG)
#本工程的名字
project(OpenGL_Lecture)
#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#file(GLOB ASSETS "./assets" )
#file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

# 包含第三方库的头文件目录
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)

# 链接第三方库的目录
link_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib)

# 添加子目录（这些目录有自己的CMakeLists.txt）
add_subdirectory(wrapper)      # 包含wrapper子项目
add_subdirectory(application)  # 包含application子项目
add_subdirectory(glframework)  # 包含glframework子项目

# 编译main.cpp和glad.c，生成可执行文件openglStudy.exe
add_executable(openglStudy "main.cpp" "glad.c")

# 链接所需的库
target_link_libraries(openglStudy glfw3.lib wrapper app fw)

# ========= 资源管理（关键） =========
# 拷贝 assets 到运行目录
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_BINARY_DIR}
)

# 注入资源根目录给 C++,创建一个CMake变量ASSETS_RUNTIME_DIR，值为构建目录/assets
set(ASSETS_RUNTIME_DIR "${CMAKE_BINARY_DIR}/assets")

# 为openglStudy可执行文件定义预处理器宏
# DEBUG: 定义DEBUG宏，代码中可以用#ifdef DEBUG判断
# ASSETS_DIR: 定义为资源目录的路径
target_compile_definitions(openglStudy PRIVATE
    DEBUG
    ASSETS_DIR="${ASSETS_RUNTIME_DIR}"
)