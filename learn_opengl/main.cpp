#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Shader.h"
#include "stb_image.h"
#include<algorithm>
#include "Texture.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int nows[] = { 3, 10, 17 };
bool flag[] = { true, true, true };
float odds = 0.2;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Shader& programShader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		odds -= 0.001;
		odds = std::clamp(odds, 0.0f, 1.0f);
		printf("%f\n", odds);
		programShader.setFloat("odds", odds);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		odds += 0.001;
		printf("%f\n", odds);
		odds = std::clamp(odds, 0.0f, 1.0f);
		programShader.setFloat("odds", odds);
	}
}

//����
void changeColor(float* arr)
{
	//Ӧ�ô���ѧ�ǶȽ���������Ǵ���ĽǶȡ�
	float timeValue = glfwGetTime();
	float value[] = { sin(timeValue) / 2.0f + 0.5f, sin(timeValue + 2.0944) / 2.0f + 0.5f,sin(timeValue + 2.0944 + 2.0944) / 2.0f + 0.5f };
	int indice[] = { 3,4,5,10,11,9,17,15,16};
	for (int i = 0; i < 9; i+=3)
	{
		for (int j = 0; j < 3; j++)
		{
			arr[indice[i+j]] = value[j];
		}
	}
}



//����ʵ�ַ���2��һ��һ��Ҫע��,flagҪ�������������⻥��Ӱ�졣�Լ�flag��nowsһ��Ҫ����Ϊȫ�֣�
// ��Ҫɵ������ÿ��ִ�к��������¶���һ�Σ��Ǿ��൱�ڰ׸���
void changeColor2(float* arr)
{
	int now, next, next_next;
	float timeValue = glfwGetTime();
	float value = sin(timeValue) / 2.0f + 0.5f;
	for (int i = 0; i < 3; i++)
	{
		int begin = 3 + i * 6;
		now = nows[i];
		next = (now + 1) % 3 + begin;
		next_next = (next + 1) % 3 + begin;
		
		if (flag[i])
		{
			arr[now] = value;
			arr[next] = 1 - value;
			arr[next_next] = 0;
		}
		else
		{
			arr[now] = 1- value;
			arr[next] = value;
			arr[next_next] = 0;
		}
		if (arr[now] < 1e-3)
		{
			nows[i] = (now + 1) % 3 + begin;
			flag[i] = !flag[i];
		}
	}
	//printf("%f %f %f\n", arr[3], arr[4], arr[5]);
}


int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow* window = glfwCreateWindow(800,600,"learn_opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "fail to create opengl window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	const char* vertexPath = "D:\\learning_codes\\CG\\learn_opengl\\learn_opengl\\v_shader.glsl";
	const char* fragPath = "D:\\learning_codes\\CG\\learn_opengl\\learn_opengl\\f_shader.glsl";
	
	Shader shaderProgram(vertexPath, fragPath);

	Texture texture1;

	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	texture1.loadData(".\\container.jpg");

	Texture texture2;

	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	texture2.loadData(".\\awesomeface.png");

	shaderProgram.use();
	shaderProgram.setInt("ourTexture1", 0);
	shaderProgram.setInt("ourTexture2", 1);
	shaderProgram.setFloat("odds", 0.2);


	//��ʼ��VAOs��VBOs
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);
	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	float vertices_1[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	unsigned int indices[] = {

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	//GL_ARRAY_BUFFER���ǲ��vbos[0]����u�̡�
	
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//ָ����θ�v_shader�е�locationΪ0��1�Ķ��㴫������
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	texture1.bindTextureUnit(0);
	texture2.bindTextureUnit(1);
	


	//��Ⱦ����ѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, shaderProgram);

		//clear���������
		//clear color���Ǹ���Ļ��ɫ

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int transloc = glGetUniformLocation(shaderProgram.ID, "trans");
		glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = std::fabs(static_cast<float>(sin(glfwGetTime())));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(trans));
		//整个光栅化的过程，都被封装在下面这个函数了
		//前面的其实都是配置
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}
