#version 300 es
precision highp float;
layout( location = 0) in vec3 aPos;
layout( location = 1) in vec2 aTexcoord;

out vec2 vTexcoord;


void main()
{
    vTexcoord = aTexcoord;
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}