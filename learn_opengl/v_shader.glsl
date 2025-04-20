#version 460
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;


out vec3 normal;
out vec3 fragPos;
out vec3 ti_lightPos;


void main()
{
	gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	fragPos = vec3(view*model*vec4(aPos, 1.0f));
	normal = transpose(inverse(mat3(view*model)))*aNormal;
	ti_lightPos = vec3(view*vec4(lightPos, 1.0f));
}
