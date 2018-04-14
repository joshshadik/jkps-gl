#version 430 core

in vec2 vTexcoord;
in vec3 vNormal;

layout(std140, binding = 0) uniform Common
{
	mat4 view;
	mat4 projection;
};

layout(std140, binding = 1) uniform PBR
{
	vec4 baseColorFactor;
	float metallicFactor;
	float roughnessFactor;
};

uniform mat4 model;
uniform vec4 diffuseColor;

uniform sampler2D uDiffuse;
uniform sampler2D uNormalTex;

out vec4 GL_FragColor;

void main(void) {
    vec4 col = texture(uDiffuse, vTexcoord.st); // vec4(vTexcoord.st, 0.0, 1.0); //
    col.a = 1.0;
	// col.rg = vTexcoord.xy;
	// col.b = 0.0;
    GL_FragColor = col;
}