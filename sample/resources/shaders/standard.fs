#version 430 core

in vec2 vTexcoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBinormal;

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

uniform sampler2D uDiffuse;
uniform sampler2D uNormalTex;
uniform sampler2D uOcclusionTex;

out vec4 GL_FragColor;

void main(void) {
    vec4 col = texture(uDiffuse, vTexcoord.st); // vec4(vTexcoord.st, 0.0, 1.0); //
	col *= diffuseFactor;

	vec3 n = texture(uNormalTex, vTexcoord.st).xyz;
	n = normalize(n * 2.0 - 1.0);
	n = n.x * vTangent.xyz + n.y * vBinormal.xyz + n.z * vNormal.xyz;

	float d = dot(n, normalize(vec3(0.0, 1.0, 1.0)));

	col.rgb *= d;

	vec4 occ = texture(uOcclusionTex, vTexcoord.st);
	col.rgb = col.rgb * occ.r;

    GL_FragColor = col;
}