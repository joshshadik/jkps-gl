#version 300 es
precision highp float;

in vec2 vTexcoord;

uniform sampler2D uColorTex;
uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uMetalRoughOccTex;

uniform vec3 uLightDirection;

out vec4 color;

void main()
{
    vec4 col = texture(uColorTex, vTexcoord.st);
    vec4 pos = texture(uPositionTex, vTexcoord.st);
    vec4 n = texture(uNormalTex, vTexcoord.st);
    vec4 mro = texture(uMetalRoughOccTex, vTexcoord.st);

    float d = max(dot(-uLightDirection, n.xyz), 0.0);

    if(n.w > 0.0 )
    {
        col.rgb *= d;
        col.rgb = col.rgb * 0.5 + col.rgb * mro.z;
    }


    color = col;
}