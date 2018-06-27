#version 300 es
precision highp float;

in vec2 vTexcoord;
in vec4 vWorldPos;

uniform vec4 diffuseColor;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;
layout(location = 3) out vec4 metalRoughOcc;


void main()
{
    color = diffuseColor;
	position = vWorldPos;
	normal = vec4(1.0, 0.0, 0.0, 0.0);
	metalRoughOcc = vec4(0.0, 0.0, 1.0, 1.0);
}