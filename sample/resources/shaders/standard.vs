#version 430 core
layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;
layout( location = 2) in vec3 aNormal;

layout(std140, binding = 0) uniform Common
{
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 color;
};

out vec2 vTexcoord;
out vec3 vNormal;

void main(void)
{
    vTexcoord = aTexcoord; // (projection * view * model * vec4(aPos.xyz, 1.0)).xy; //(aPos.xy + vec2(1.0, 1.0)) * 0.5;
    vNormal = aNormal;

    vec4 pos = projection * view * model * vec4(aPos.xyz, 1.0);

    gl_Position = pos; //vec4(vTexcoord * 2.0 - 1.0, 0.0, 1.0); // ; 
}