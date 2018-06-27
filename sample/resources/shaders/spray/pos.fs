#version 300 es
precision highp float;

in vec2 vTexcoord;

uniform sampler2D posTex;
uniform sampler2D velTex;

uniform float deltaTime;

layout(location = 0) out vec4 position;

void main()
{
    vec4 pos = texture(posTex, vTexcoord);
    vec4 vel = texture(velTex, vTexcoord);

    pos = pos + vel * deltaTime;

    position = pos;
}