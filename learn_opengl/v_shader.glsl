#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 texCoord;
uniform float offset;
uniform mat4 trans;

void main()
{
	gl_Position = trans*vec4(aPos.x + offset, aPos.y, aPos.z, 1.0f);
	texCoord = aTexCoord;
	ourColor = aColor;
}



