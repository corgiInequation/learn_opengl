#version 460
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 fragColor;


void main()
{
	gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	vec3 fragPos = vec3(view*model*vec4(aPos, 1.0f));
	vec3 normal = transpose(inverse(mat3(view*model)))*aNormal;
	vec3 ti_lightPos = vec3(view*vec4(lightPos, 1.0f));

	//使用的是摄像坐标系，所以摄像机肯定是在(0,0,0)
	vec3 viewPos = vec3(0.0f);
	//环境光照
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//漫反射光照
	vec3 in_vec = normalize(ti_lightPos - fragPos);
	vec3 norm = normalize(normal);	
	vec3 diffuse = max(dot(in_vec, norm), 0) * lightColor;

	//镜面反射光照
	vec3 reflect_vec = reflect(-in_vec, norm);
	vec3 view_vec = normalize(viewPos - fragPos);
	float spec = max(dot(reflect_vec, view_vec), 0);
	spec = pow(spec, 128)*0.8;
	vec3 specular = spec*lightColor;

	fragColor = vec4((ambient + diffuse + specular) * objectColor, 0.0f);
}



