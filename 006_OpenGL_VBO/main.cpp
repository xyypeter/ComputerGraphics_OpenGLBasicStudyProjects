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

//VBO的创建和销毁
void prepareVBO() {
	//1 创建一个vbo，此处还没有真正分配显存
	GLuint vbo = 0;
	GL_CALL(glGenBuffers(1, &vbo));

	//2 销毁一个vbo
	GL_CALL(glDeleteBuffers(1, &vbo));

	//3 创建n个vbo
	GLuint vboArr[] = { 0,0,0 };
	GL_CALL(glGenBuffers(3, vboArr));

	//4 销毁n个vbo
	GL_CALL(glDeleteBuffers(3, vboArr));
}

//VBO数据绑定和传输
void prepare() {
	//NDC坐标数据
	float vertices[] = {
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	//1 生成一个vbo
	GLuint vbo = 0;    
	GL_CALL(glGenBuffers(1, &vbo));
    //2 绑定当前vbo到opengl状态机的当前vbo插槽上,---GL_ARRAY_BUFFER:表示当前vbo这个插槽
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//3 向当前vbo传输数据，也是在开辟显存
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

//VBO多属性数据存储
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
	   0.0f,  0.0f, 1.0f
	};
	//2 为位置&颜色数据各自生成一个vbo
	GLuint posVbo = 0, colorVbo = 0;
	GL_CALL(glGenBuffers(1, &posVbo));
	GL_CALL(glGenBuffers(1, &colorVbo));
	//3 给两个分开的vbo各自填充数据
	//position填充数据
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
	//color填充数据
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));
}

//交叉式(将所有属性存储至一个VBO中)
void prepareInterleavedBuffer() {
	float vertices[] = {
	   -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	GLuint vbo = 0;
	//创建一个VBO
	GL_CALL(glGenBuffers(1, &vbo));
	//绑定当前vbo到opengl状态机的当前vbo插槽上
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//给VBO填充数据
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
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

	//VBO创建测试
	//prepareVBO();

	//VBO数据绑定和传输测试
	//prepare();

	//VBO多属性数据存储
	prepareSingleBuffer();

	while (app->update()) {
		//执行opengl画布清理操作
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		//渲染操作
		//TODO-----------
	}
	
	app->destroy();
	return 0;

}