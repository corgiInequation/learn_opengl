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
int change = 1;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(100, 100, width, height);
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
	//��Щ�����ڳ�ʼ������
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
	
	//opengl����غ���һ����˵�ǲ���ֱ�ӵ��õģ��������ҵ����ַ��Ȼ���ٽ��丳ֵ��һ���Լ�����ĺ�������
	//���Loader���൱�ڰ����е�opengl�����к�����load���ڴ����档glfwGetProcAddress����Ѱ�Һ����ĵ�ַ
	//֮��opengl�ĺ����Ϳ���ֱ��ʹ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//֮ǰע���ֻ�Ǵ��ڣ�����������ӿڣ��ӿڿ��ܱȴ���С����Ϊ��ȫ�����괰���еĶ���
	//ǰ���������������Ǵ������½ǵ�λ��
	glViewport(0, 0, 800, 600);
	
	//���ڵĴ�С�����任���ӿڵĴ�СҲҪ�任
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	const char* vertexPath = "D:\\learning_codes\\CG\\learn_opengl\\learn_opengl\\v_shader.glsl";
	const char* fragPath = "D:\\learning_codes\\CG\\learn_opengl\\learn_opengl\\f_shader.glsl";
	
	Shader shaderProgram(vertexPath, fragPath);

	//����1
	Texture texture1;

	//���������һЩ����
	//GL_MIRRORED_REPEAT������������������
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture1.setParai(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	texture1.loadData(".\\container.jpg");

	//����2
	Texture texture2;

	//���������һЩ����
	//GL_MIRRORED_REPEAT������������������
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture2.setParai(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//��������
	texture2.loadData(".\\awesomeface.png");

	//��������sampler������Ӧ�������Ԫ
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
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//ָ����θ�v_shader�е�locationΪ0��1�Ķ��㴫������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//��Ⱦ����ѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, shaderProgram);

		//clear���������
		//clear color���Ǹ���Ļ��ɫ
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		//changeColor(vertices_1);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);

		texture1.bindTextureUnit(0);
		texture2.bindTextureUnit(1);
		

		glBindVertexArray(VAOs[0]);
		shaderProgram.use();
		//glDrawArrays(GL_TRIANGLES, 0, 3);
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
