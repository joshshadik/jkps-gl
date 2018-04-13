#version 430 core
layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;

layout(std140, binding = 0) uniform Common
{
	mat4 model;
	mat4 view;
	mat4 projection;
	vec4 color;
};

out vec2 vTexcoord;

void main(void)
{
    vTexcoord = aTexcoord; // (projection * view * model * vec4(aPos.xyz, 1.0)).xy; //(aPos.xy + vec2(1.0, 1.0)) * 0.5;

    gl_Position =  projection * view * model * vec4(aPos.xyz, 1.0); 
}