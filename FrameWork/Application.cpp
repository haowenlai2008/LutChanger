#include "Shader.h"
#include "glfw3.h"
#include "glad.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>
using namespace std;
GLFWwindow* glWindow = nullptr;
//加载纹理
template <typename S1 = std::string>
void LoadTexture(unsigned int &texture, S1&& pic)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//读取图片的时候对Y轴进行反转
	unsigned char *data = stbi_load((TEXTURE_PATH + std::forward<std::string>(pic)).c_str(), &width, &height, &nrChannels, 0);
	std::cout << "nrChannels = " << nrChannels << endl;
	if (data)
	{
		//位深度为24，3个通道(jpg
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//位深度为32，4个通道(png
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	data = nullptr;
}

template <typename S1 = std::string>
void LoadTextureAbsolute(unsigned int& texture, S1 && pic)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);//读取图片的时候对Y轴进行反转
	unsigned char* data = stbi_load(std::forward<std::string>(pic).c_str(), &width, &height, &nrChannels, 0);
	std::cout << "nrChannels = " << nrChannels << endl;
	if (data)
	{
		//位深度为24，3个通道(jpg
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//位深度为32，4个通道(png
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	data = nullptr;
}
void BaseInit()
{
	glfwInit();//初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//配置GLFW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	float screenWidth = 800.0f;
	float screenHeight = 640.0f;
	//创建窗口
	glWindow = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	if (glWindow == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(glWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

class RenderBufferStructure
{
public:
	unsigned int bufferID = -1;
	unsigned int textureID = -1;
	unsigned int rbo = -1;
	RenderBufferStructure() {}
	RenderBufferStructure(int weight, int height) 
	{
		glGenFramebuffers(1, &bufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferID);
		//添加一个颜色附件
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, weight, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
		//为深度和模板附件创建一个渲染缓冲对象 create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, weight, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		//创建帧缓冲，添加所有的附件 now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	~RenderBufferStructure() {}
};


void LUTDeal(vector<string> path)
{
	float vertices[] = {
		//     ---- 位置 ----     - 纹理坐标 -
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,    // 右上
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // 右下
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // 左下
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	//编译着色器
	Shader ourShader("vertex_3.vs", "lookup_base.fs");
	ourShader.use();//glUseProgram(shaderProgram);
	unsigned int VAO, VBO, EBO;

	//顶点数组
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//绑定顶点数组缓存
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//索引缓存
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	//最后一个参数是数据的起点
	glEnableVertexAttribArray(1);

	Shader ourShader2("vertex_3.vs", "lookup_base_fragment_shader.fs");
	unsigned int VAO2, VBO2, EBO2;
	//顶点数组
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	//绑定顶点数组缓存
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//索引缓存
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	//最后一个参数是数据的起点
	glEnableVertexAttribArray(1);

	RenderBufferStructure rbs(64, 64);

	glViewport(0, 0, 64, 64);

	for (auto& p : path)
	{
		//生成纹理
		unsigned int texture1;
		LoadTextureAbsolute(texture1, p);	//加载纹理
		ourShader.use();
		glBindFramebuffer(GL_FRAMEBUFFER, rbs.bufferID);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//draw
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		ourShader2.use();
		ourShader2.setFloat("uQuadDim", 8.0);
		ourShader2.setFloat("uQuadPixel", 64.0);
		ourShader2.setInt("inputImageTexture", 0);
		ourShader2.setInt("inputImageTexture2", 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//draw
		glBindVertexArray(VAO2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rbs.textureID);//绑定纹理
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);//绑定纹理
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		int data_size = 64 * 64 * 4;
		GLubyte* pixels = new GLubyte[data_size];
		glReadPixels(0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		stbi_flip_vertically_on_write(true);
		cout << p << endl;
		stbi_write_png(p.c_str(), 64, 64, 4, pixels, 0);
		stbi_image_free(pixels);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glfwTerminate();
}
void MainLoop()
{
	
}

int main(int argc, char* argv[])
{
	vector<string> picPath;
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			string p = string(argv[i], argv[i] + strlen(argv[i]));
			picPath.push_back(p);
		}
			
	}
	BaseInit();
	LUTDeal(picPath);
	return 0;;
}