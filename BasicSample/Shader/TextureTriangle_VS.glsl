#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
uniform float myPos;
uniform mat4 transform;
void main()
{
	gl_Position = transform * vec4(aPos.x + myPos, aPos.y, aPos.z, 1.0);
	texCoord = aTexCoord;
}