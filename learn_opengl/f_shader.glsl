#version 460

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 fragPos;
in vec3 normal;
in vec3 ti_lightPos;


out vec4 fragColor;

void main()
{
	vec3 test_viewPos = vec3(0.0f);
	vec3 v = viewPos;
	//环境光照
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//漫反射光照
	vec3 in_vec = normalize(ti_lightPos - fragPos);
	vec3 norm = normalize(normal);	
	vec3 diffuse = max(dot(in_vec, norm), 0) * lightColor;

	//镜面反射光照
	vec3 reflect_vec = reflect(-in_vec, norm);
	vec3 view_vec = normalize(test_viewPos - fragPos);
	float spec = max(dot(reflect_vec, view_vec), 0);
	spec = pow(spec, 32)*0.5;
	vec3 specular = spec*lightColor;

	fragColor = vec4((ambient + diffuse + specular) * objectColor, 0.0f);
}