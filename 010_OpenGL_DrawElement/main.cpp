#include <iostream>
#include <glad/glad.h>//坑：头文件引用，glad需要放在GLFW上
#include <GLFW/glfw3.h>
#include "wrapper/checkError.h"
#include "application/application.h"
using namespace std;

GLuint vao, program;//将这两个参数声明为全局变量，使用InterleavedBuffer

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVAO() {
	//1 准备位置数据和索引数据
	//位置数据
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	//索引数据
	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};
	//2 VBO创建
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4 VAO创建
	//此处不需要创建vao,因为代码第8行,已经含有全局变量的vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//此行代码在36行已经编写是否可以删除？？？可以删除
	glEnableVertexAttribArray(0);//位置属性放在0号位，需要先激活
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//void glVertexAttribPointer(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const void* pointer);
	// index:要描述的第几个属性
	// size:这个属性包含几个数字
	// type:这个属性的类型是什么0
	// normalized:是否归一化，暂时无需考虑直接填写GL_FALSE
	// stride:  每个顶点的数据步长
	// pointer: 这个属性在每个顶点数据内的偏移量
	// 
	//5.2 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);//此行代码在41行已经编写是否可以删除？？？一定不可以  为什么50行可以删除？？？
	//因为：加入删除了第50行代码，glVertexAttribPointer函数的执行会查询VBO是否绑定，即便删除50行的代码，也会向上查找，找到36行的绑定
	//但是从45行---61之间，没有类似函数去检查是否绑定ebo,所以无法删除
	// 
	//知识补充： 
	// glVertexAttribPointer会从当前绑定的VBO中读取数据，但VBO的绑定状态不会被VAO记录。VAO只会记录属性指针的配置（如glVertexAttribPointer的参数），
	// 而不是VBO本身。因此，即使解绑VBO，只要VAO的属性配置正确，渲染仍能正常工作。
	// 
	//
	//6.解绑0号位
	glBindVertexArray(0);
}

void prepareShader() {
	//1 完成vs与fs的源代码，并且装入字符串
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
	//2 创建Shader程序（vs、fs）
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	//3 为shader程序输入shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	int success = 0;
	char infoLog[1024];
	//4 执行shader代码编译 
	glCompileShader(vertex);
	//检查vertex编译结果
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --VERTEX" << "\n" << infoLog << std::endl;
	}
	glCompileShader(fragment);
	//检查fragment编译结果
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "Error: SHADER COMPILE ERROR --FRAGMENT" << "\n" << infoLog << std::endl;
	}
	//5 创建一个Program壳子
	program = glCreateProgram();
	//6 将vs与fs编译好的结果放到program这个壳子里
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	//7 执行program的链接操作，形成最终可执行shader程序
	glLinkProgram(program);
	//检查链接错误
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
	}
	//清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void render() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	//1 绑定当前的program
	glUseProgram(program);
	//2 绑定当前的vao
	glBindVertexArray(vao);
	//3 发出绘制指令
	//由于现在使用ebo进行绘制，所以需要使用索引绘制函数，替换glDrawArrays()函数
	//void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
	//mode:绘制模式GL_TRIANGLES
	//count:绘制索引数组中的几个点
	//type:索引的数据类型
	//indices:
	// 如果使用ebo,通常填写0
	// 如果使用ebo,不前填写0，则表示索引内偏移
	// 如果不适用ebo,可以直接传入索引数组
	//
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//从第0个索引开始绘制
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);//只使用前三个索引
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(int)*3));//加三个偏移量，使用后三个索引

	//unsigned int indices[] = {
	//0, 1, 2,
	//2, 1, 3
	//};
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);//直接使用数据索引，传给CPU端开始绘制
	glBindVertexArray(0);//解除VAO绑定
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

	//测试部分
	prepareShader();
	prepareVAO();
	while (app->update()) {
		render();
	}
	//推出
	app->destroy();
	return 0;

}