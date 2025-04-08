#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float odds;

void main()
{
    FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture2, vec2(1.0 - texCoord.x, texCoord.y)), odds);
}