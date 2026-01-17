# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ComputerGraphics_OpenGL")

# 包含子项目。
#add_subdirectory ("ComputerGraphics_OpenGL")
#add_subdirectory ("000_CmakeStudy")
#add_subdirectory ("001_OpenGL_EngineeredTemplate")
#add_subdirectory ("002_GLFW_WINDOW")
#add_subdirectory ("003_OpenGL_TryFunctions")
#add_subdirectory ("004_OpenGL_ErrorChecker")
#add_subdirectory ("005_OpenGL_Application")
#add_subdirectory ("006_OpenGL_VBO")
#add_subdirectory ("007_OpenGL_VAO")
#add_subdirectory ("008_OpenGL_Shader")
#add_subdirectory ("009_OpenGL_DrawArray")
#add_subdirectory ("010_OpenGL_DrawElement")
#add_subdirectory ("011_OpenGL_ColorfulTriangle")
#add_subdirectory ("012_OpenGL_ShaderClass")
#add_subdirectory ("013_OpenGL_Uniform")
#add_subdirectory ("014_OpenGL_Exercise-1")
#add_subdirectory ("015_OpenGL_Exercise-2")
#add_subdirectory ("016_OpenGL_Exercise-3")
#add_subdirectory ("017_OpenGL_Texture")
#add_subdirectory ("018_OpenGL_Texture-Exercise-1")
#add_subdirectory ("019_OpenGL_Texture-Class")
#add_subdirectory ("020_OpenGL_Texture-Exercise-2")
#add_subdirectory ("021_OpenGL_Texture-Mipmap-Test")
#add_subdirectory ("022_OpenGL_Texture-Mipmap-ShaderTest")
add_subdirectory ("023_OpenGL_Texture-Mipmap-Auto")
