#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include"Texture.h"

Texture::Texture() {
    glGenTextures(1, &id);
}

void Texture:: bindTextureUnit(int i)
{
	//相当于把id的信息存入GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, id);
}
void Texture:: setParai(GLenum target, GLenum pname, GLint param)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(target, pname, param);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void Texture:: loadData(const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	//加载纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		GLenum format = GL_RGB;
		if (nrChannels == 4) format = GL_RGBA;
		else if (nrChannels == 1) format = GL_RED;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}




