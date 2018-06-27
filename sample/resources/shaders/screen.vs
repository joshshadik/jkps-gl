#version 300 es
precision highp float;

layout( location = 1) in vec2 aTexcoord;

out vec2 vTexcoord;

void main(void)
{
    vTexcoord = aTexcoord;

    gl_Position = vec4(vTexcoord * 2.0 - 1.0, 0.0, 1.0);
}