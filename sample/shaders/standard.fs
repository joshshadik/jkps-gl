#version 430 core

in vec2 vTexcoord;

layout(std140, binding = 0) uniform Common
{
	mat4 model;
	mat4 view;
	mat4 projection;
	vec4 color;
};

uniform sampler2D uDiffuse;

out vec4 GL_FragColor;

void main(void) {
    vec4 col = texture(uDiffuse, vTexcoord.st); // vec4(vTexcoord.st, 0.0, 1.0); //
    col.a = 1.0;
    GL_FragColor = col;
}