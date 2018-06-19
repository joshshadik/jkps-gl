#version 300 es
precision highp float;

layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;

layout(std140)  uniform Common
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;
uniform vec4 diffuseColor;

out vec2 vTexcoord;
//out vec3 vNormal;
out vec4 vWorldPos;

void main(void)
{
    vTexcoord = aTexcoord;
	//vNormal = normalize((model * vec4(aNormal.xyz, 0.0)).xyz);

	vWorldPos = model * vec4(aPos.xyz, 1.0);
	vec4 pos = projection * view * vWorldPos;

    gl_Position = pos; // vec4(vTexcoord * 2.0 - 1.0, 0.0, 1.0);
}