#include <iostream>
#include <glad/glad.h>//坑：头文件引用，glad需要放在GLFW上
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/application.h"
using namespace std;

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareSingleBuffer() {
	//1 准备顶点位置数据与颜色数据
	float positions[] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};
	float colors[] = {
	   1.0f, 0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
	   0.0f, 0.0f, 1.0f
	};
	//2 为位置&颜色数据各自生成一个vbo,绑定对应内容
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

	//3 生成VAO并且绑定
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//4 分别将位置/颜色属性的描述信息加入vao中
	//4.1描述信息加入vao中
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);//此行的目的是，只有绑定了posVbo,以下属性才会生效
	glEnableVertexAttribArray(0);//位置属性存放在vao属性的0号位置，需要先激活
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//4.2颜色信息加入vao中
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);//此行的目的是，只有绑定了posVbo,以下属性才会生效
	glEnableVertexAttribArray(1);//位置属性存放在vao属性的0号位置，需要先激活
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//5 将当前VAO进行解绑一下
	glBindVertexArray(0);
}

void prepareInterleavedBuffer() {
	//1 准备好Interleaved数据（位置+颜色）
	float vertices[] = {
		   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	//2 创建唯一的vbo
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//3 创建并绑定vao
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));//此行的目的是，只有绑定了posVbo,以下属性才会生效
	//4 为vao加入位置和颜色的描述信息
	//4.1 位置描述信息
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

	//4.2 颜色描述信息
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

	//5 扫尾工作：解绑当前vao
	glBindVertexArray(0);
}

void prepareShader() {
	//1 完成vs(Vertex Shader)与fs(Fragment Shader)的源代码，并且装入字符串
	//字符串的本质是字符数组
	//C/C++中，字符串通常以 字符指针（const char* 或 char*） 的形式使用，并隐式包含一个终止符 \0
	const char* vertexShaderSource =
		"#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	//2 创建Shader程序(Vs->vertexShader，表示和Fs->表示fragmentShader)
	//GLuint glCreateShader(GLenum type)表示创建某一种shader程序，并返回句柄ID
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	
	//3 创建Shader程序输入shader代码
	//void glShaderSource(
	//	GLuint shader,          // 着色器对象句柄
	//	GLsizei count,          // 字符串数量
	//	const GLchar * *string, // 指向着色器源代码字符串数组的指针
	//	const GLint * length    // 各字符串长度数组（可选，填写NULL）
	//);
	glShaderSource(vertex,1,&vertexShaderSource,NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	//4 执行shader代码编译,且检查vertex编译结果
	int success = 0;
	char infoLog[1024];
	//**********检查vertexShader
	glCompileShader(vertex);
	//void glCompileShader(GLuint shader)是对shader程序执行编译操作
	glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
	//检查vertexShader的编译结果是否正确GL_COMPILE_STATUS，返回结果指针
	//void glGetShaderiv(
    //GLuint shader,     // 着色器对象句柄
    //GLenum pname,      // 要查询的参数名
    //GLint* params      // 存储查询结果的指针
    //	);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		//检查与 vertex 着色器相关的编译信息。如果在编译过程中发生错误或警告，glGetShaderInfoLog 将把这些信息存放到 infoLog 中
		//void glGetShaderInfoLog(
		//	GLuint shader,       // 着色器对象句柄
		//	GLsizei maxLength,   // 日志缓冲区最大长度
		//	GLsizei * length,    // 实际日志长度（可选，可传 NULL）
		//	GLchar * infoLog     // 存储日志的字符数组
		//);
		std::cout << "Error: SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
	}
	//**********检查fragmentShader
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
	}

	//5 开始链接部分，首先创建一个Program壳子(将vertexShader想象为调料，fragmentShader想象成食材，Program类似砂锅)
	//GLuint glCreateProgram();创建最终可执行程序对象，返回编号(句柄)
	GLuint program = 0;
	program = glCreateProgram();
	
	//6 将Vs和Fs编译好的结果存在program的壳子里(调料(vertexShader)+食材(fragmentShader)放入砂锅开始煲汤)
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	//7 将program进行链接操作，形成最终的可执行shader程序
	glLinkProgram(program);
	//7.1检查上述链接是否存在错误
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
	}
	//8 执行清理操作(最终将调料和食材经过使用，汤出锅，其他的东西需要清理->倒掉)
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

int main() {
	//窗口初始化
	if (!app->init(800, 600)) {
		return -1;
	}
	//调用窗口响应和键盘响应
	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
 
	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //测试shader代码
	prepareShader();

	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
		//TODO-----------
	}
	
	app->destroy();
	return 0;

}