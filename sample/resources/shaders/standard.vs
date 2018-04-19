#version 430 core
layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;
layout( location = 2) in vec3 aNormal;
layout( location = 3) in vec4 aTangent;

layout(std140, binding = 0) uniform Common
{
	mat4 view;
	mat4 projection;
};

layout(std140, binding = 32) uniform PBR
{
	vec4 diffuseFactor;
	float metallicFactor;
	float roughnessFactor;
};

uniform mat4 model;
uniform vec4 diffuseColor;

out vec2 vTexcoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBinormal;
out vec4 vWorldPos;

void main(void)
{
    vTexcoord = aTexcoord;
	vNormal = normalize((model * vec4(aNormal.xyz, 0.0)).xyz);
	vTangent = normalize((model * vec4(aTangent.xyz, 0.0)).xyz);
	vBinormal = cross(vNormal, vTangent) * aTangent.w;

	vWorldPos = model * vec4(aPos.xyz, 1.0);
	vec4 pos = projection * view * vWorldPos;

    gl_Position = pos; // vec4(vTexcoord * 2.0 - 1.0, 0.0, 1.0);
}