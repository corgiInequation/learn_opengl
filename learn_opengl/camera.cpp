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
#include "camera.h"

Camera::Camera(glm::vec3 gg, glm::vec3 tt, glm::vec3 poss, float fovv, float vertical_anglee, float level_anglee, float sensitivityy, float speedd)
{
	g = glm::normalize(gg);
	t = glm::normalize(tt);
	gxt = glm::normalize(glm::cross(g, t));
	pos = poss;
	fov = fovv;
	vertical_angle = vertical_anglee;
	level_angle = level_anglee;
	sensitivity = sensitivityy;
	speed = speedd;
}

glm::mat4 Camera:: getViewMatrix()
{
	//涉及有关方向的向量，都需要标准化
	//但是和方向无关的，比如Pos,就不需要标准化了
	g = glm::normalize(g);
	t = glm::normalize(t);
	glm::vec3 gxt = glm::normalize(glm::cross(g, t));
	t = glm::normalize(glm::cross(gxt, g));

	glm::mat4 res = glm::mat4(1.0f);
	res[0][0] = gxt[0];
	res[1][0] = gxt[1];
	res[2][0] = gxt[2];
	res[0][1] = t[0];
	res[1][1] = t[1];
	res[2][1] = t[2];
	res[0][2] = -g[0];
	res[1][2] = -g[1];
	res[2][2] = -g[2];
	res[3][0] = -glm::dot(gxt, pos);
	res[3][1] = -glm::dot(t, pos);
	res[3][2] = glm::dot(g, pos);
	return res;
}

void Camera::processKeyBoard(Camera_Movement movement, float deltaTime)
{
	if (movement == FORWARD)
	{
		pos += speed * deltaTime * g;
	}
	else if (movement == BACKWARD)
	{
		pos -= speed * deltaTime * g;
	}
	else if (movement == RIGHT)
	{
		pos += speed * deltaTime * glm::cross(g, t);
	}
	else if (movement == LEFT)
	{
		pos -= speed * deltaTime * glm::cross(g, t);
	}
	//如果是把g改成-z方向的向量,cross(g, t)改成x方向向量，不太好。这样子就不能斜着走。
	pos.y = 0.0f;
}

void Camera::processMouseMovement(float xoffet, float yoffet)
{
	xoffet *= sensitivity;
	yoffet *= sensitivity;
	vertical_angle = std::clamp(vertical_angle + yoffet, -89.0f, 89.0f);
	level_angle = level_angle + xoffet;
	updateCameraVectors();

}

void Camera::processMouseScroll(float yoffset)
{
	fov = std::clamp(fov - yoffset, 1.0f, 60.0f);
}

void Camera::updateCameraVectors()
{
	g = glm::vec3(cos(glm::radians(vertical_angle)) * cos(glm::radians(level_angle)), sin(glm::radians(vertical_angle)), cos(glm::radians(vertical_angle)) * sin(glm::radians(level_angle)));
	g = glm::normalize(g);
	gxt = glm::normalize(glm::cross(g, t));
	t = glm::normalize(glm::cross(gxt, g));
}
