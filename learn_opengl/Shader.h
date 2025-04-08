#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class  Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragPath);

	void use();
	// uniform工具函数
	//&有两种功能，一种是引用，一种是去地址。在这里是引用的意思，可以避免拷贝，省下空间
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:

};

#endif
