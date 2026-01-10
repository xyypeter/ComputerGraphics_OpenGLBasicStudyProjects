#pragma once 
//wrapper(包装器)
//预编译宏(宏替换和预编译(解决发布时跳过开发期间的GL_CALL))
#ifdef DEBUG
#define GL_CALL(func) func;checkError();
#else
#define GL_CALL(func) func;
#endif 

void checkError();