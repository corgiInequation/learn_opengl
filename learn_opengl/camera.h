#ifndef CAMERA_H
#define CAMERA_H
//构建一个类的步骤：先写.h文件，之后ifndef, 之后就是include所需文件, 之后就是class了。目前还不需要namespace, 因为只有一个类。
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

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	//首先定义变量，摄像机需要：g,t,pos,fov,偏航角，俯仰角，以及移动速度和灵敏度
	glm::vec3 g, t, gxt ,pos;
	float fov;
	float vertical_angle, level_angle;
	float sensitivity, speed;

	//camera对象中并不会把函数的全部功能都实现，鼠标和键盘的移动量还是得再main中算
	Camera(glm::vec3 gg, glm::vec3 tt, glm::vec3 poss, float fovv, float vertical_anglee, float level_anglee, float sensitivityy, float speedd);
	glm:: mat4 getViewMatrix();
	void processMouseMovement(float xoffet, float yoffet);
	void processMouseScroll(float yoffet);
	void processKeyBoard(Camera_Movement movement, float deltaTime);
	glm::vec3 getCameraPos();
private:
	//把相机的一些向量改变
	void updateCameraVectors();
};



#endif

