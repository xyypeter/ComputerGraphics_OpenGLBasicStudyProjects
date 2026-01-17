#include <iostream>
#include "glframework/core.h"
#include "glframework/shader.h"
#include "wrapper/checkError.h"
#include "application/application.h"
using namespace std;

GLuint vao;//将这两个参数声明为全局变量，使用InterleavedBuffer

Shader* shader = nullptr;//全局的shader对象

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVAO() {
	//1 准备positions
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	//补充颜色数据
	float colors[] = {
	1.0f, 0.0f,0.0f,
	0.0f, 1.0f,0.0f,
	0.0f, 0.0f,1.0f
	};
	//保留三个顶点数据
	unsigned int indices[] = {0, 1, 2};
	//2 VBO创建
	GLuint posVbo, colorVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//5.2 加入位置颜色描述信息
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//5.3 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}

void prepareShader() {
	//shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
	//shader = new Shader(std::string(ASSETS_DIR) + "/shaders/vertex.glsl", std::string(ASSETS_DIR) + "/shaders/fragment.glsl");
	shader = new Shader(
		(std::string(ASSETS_DIR) + "/shaders/vertex.glsl").c_str(),
		(std::string(ASSETS_DIR) + "/shaders/fragment.glsl").c_str()
	);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	//1.绑定当前的program
	//glUseProgram(program);//封装好了shader类丢弃此方法
	shader->begin();
	//2.绑定当前的vao
	glBindVertexArray(vao);
	//3.发出绘制指令
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//4.解除绑定
	glBindVertexArray(0);
	//5.清理shder状态
	shader->end();       
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


