#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Shader.h"
#include "camera.h"
#include "stb_image.h"
#include<algorithm>
#include "Texture.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float odds = 0.2;

glm::vec3 pos(0.0f, 0.0f, 5.0f);
glm::vec3 gaze(0.0f, 0.0f, -1.0f);
glm::vec3 t(0.0f, 1.0f, 0.0f);
Camera camera(gaze, t, pos, 45.0f, 0.0f, -90.0f, 0.05f, 1.0f);
float lastX = 0;
float lastY = 0;
float lastTime = 0;
bool firstMouse = true;




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

//之所以有的时候又double，是因为callback函数要求的为double
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window, Shader& programShader)
{
	float currentTime = (float)glfwGetTime();
	float deltaTime =  currentTime - lastTime;
	lastTime = currentTime;
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
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyBoard(FORWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyBoard(BACKWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyBoard(RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyBoard(LEFT, deltaTime);
	}
}


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl", NULL, NULL);
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
		//     ---- 位置 ----       ---- 颜色 ----     - texcoord -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	unsigned int indices[] = {

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	//GL_ARRAY_BUFFER���ǲ��vbos[0]����u�̡�

	glBindVertexArray(VAOs[0]);
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	//ָ����θ�v_shader�е�locationΪ0��1�Ķ��㴫������
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	texture1.bindTextureUnit(0);
	texture2.bindTextureUnit(1);

	//鼠标设置
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window,mouse_callback);

	//滚轮设置
	glfwSetScrollCallback(window, scroll_callback);


	//��Ⱦ����ѭ��
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, shaderProgram);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		shaderProgram.setMat4("view", view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);
		shaderProgram.setMat4("projection", projection);

		//clear���������
		//clear color���Ǹ���Ļ��ɫ
		for (int i = 0; i < 10; i++)
		{

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i % 3 == 0)
			{
				float angle = (float)glfwGetTime()*10;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shaderProgram.setMat4("model", model);
			}
			else
			{
				float angle = 20.0f;
				model = glm::rotate(model, glm::radians(angle * i), glm::vec3(1.0f, 0.3f, 0.5f));
				shaderProgram.setMat4("model", model);
			}
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}