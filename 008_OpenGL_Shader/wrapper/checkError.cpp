#include "checkError.h"
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <assert.h>

void checkError() {
	GLenum errorCode = glGetError();
	//OpenGL提供的错误检查函数
	//此函数调用之后,会检查之前的调用是否发生问题，如有问题则会返回最近一个问题的错误码
	std::string error = "";
	if (errorCode != GL_NO_ERROR) {
		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;          //表示传递给函数的枚举参数无效
		case GL_INVALID_VALUE:  error = "INVALID_VALUE"; break;       //表示传递给函数的参数值无效
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;//表示当前的操作不允许
		case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;        //表示OpenGL无法分配足够的内存
		default:
			error = "UNKNOWN";//表示除上述错误之外的错误
			break;
		}
		std::cout << error << std::endl;
		//assert会根据传入的bool值，来决定程序是否停止  true:程序顺利运行 false:程序会断死
		assert(false);
	}
}