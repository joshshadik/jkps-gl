#version 300 es
precision highp float;

layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;

layout(std140) uniform Common
{
	mat4 view;
	mat4 projection;
	mat4 invVP;
};

uniform mat4 model;
uniform vec4 diffuseColor;

uniform sampler2D posTexture;
uniform int texSize;
uniform float size;

out vec2 vTexcoord;
out vec4 vWorldPos;

void main(void)
{
    vTexcoord = aTexcoord;

	vec2 posUV = vec2( gl_InstanceID % texSize,  gl_InstanceID / texSize ) / float(texSize);
	vec4 wPos = texture(posTexture, posUV);
	wPos.w = 1.0;

	vWorldPos = wPos;
	vec4 pos = (projection * view * model * vWorldPos) ;

	pos += vec4(aPos.xy * 2.0 - 1.0, 0.0, 0.0) * size;

    gl_Position = pos;
}