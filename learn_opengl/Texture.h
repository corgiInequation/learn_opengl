#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture {
public:
	unsigned int id;
	Texture();
	void setParai(GLenum target, GLenum pname, GLint param);
	void loadData(const char* path);
	void bindTextureUnit(int i);


};


#endif


