#include <iostream>
#include "glframework/core.h"
#include "glframework/shader.h"
#include "wrapper/checkError.h"
#include "application/application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "application/stb_image.h"
using namespace std;

GLuint vao;//将这两个参数声明为全局变量，使用InterleavedBuffer
GLuint texture;
Shader* shader = nullptr;//全局的shader对象

void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVAO() {
	//1 准备positions colors
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
		0.5f, 0.5f,0.5f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};
	//保留三个顶点数据
	//unsigned int indices[] = {0, 1, 2};
	//2 VBO创建
	GLuint posVbo, colorVbo,uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

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

	//5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}

void prepareShader() {
	//shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
	shader = new Shader((std::string(ASSETS_DIR) + "/shaders/vertex.glsl").c_str(), (std::string(ASSETS_DIR) + "/shaders/fragment.glsl").c_str());
}

void prepareTexture() {
	//1 stbImage 读取图片
	int width, height, channels;
	//--反转y轴
	stbi_set_flip_vertically_on_load(true);
	//unsigned char* data = stbi_load("assets/textures/hinata.jpg", &width, &height, &channels, STBI_rgb_alpha);
	unsigned char* data = stbi_load((std::string(ASSETS_DIR) + "/textures/goku.jpg").c_str(), &width, &height, &channels, STBI_rgb_alpha);
	//2 生成纹理并且激活单元绑定
	//--激活纹理单元--
	glGenTextures(1, &texture);
	//--绑定纹理对象--
	glActiveTexture(GL_TEXTURE0);
	//3 传输纹理数据,开辟显存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//4 设置纹理的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//5 设置纹理的包裹方式
	//  GL_REPEAT	        重复纹理(默认)   地板、墙壁等无缝贴图
	//	GL_MIRRORED_REPEAT	镜像重复纹理	    需要对称重复的图案
	//	GL_CLAMP_TO_EDGE	拉伸边缘像素	    避免接缝（如UI贴图）
	//	GL_CLAMP_TO_BORDER	使用指定边框颜色	特殊效果（如遮罩）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v
	
	//***释放数据
	stbi_image_free(data);
}

void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	//1 绑定当前的program
	//glUseProgram(program);//封装好了shader类丢弃此方法
	shader->begin();

	shader->setInt("sampler", 0);
	shader->setFloat("time", glfwGetTime());//给名称为time的uniform传入参数(当前时间)

	//2 绑定当前的vao
	glBindVertexArray(vao);
	//3 发出绘制指令
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);//解除绑定
	shader->end();       //清理shder状态
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
	prepareTexture();//纹理图片的读取
	while (app->update()) {
		render();
	}
	//推出
	app->destroy();
	return 0;

}