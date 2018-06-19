#version 300 es
precision highp float;

in vec2 vTexcoord;

layout(std140) uniform Common
{
	mat4 view;
	mat4 projection;
	mat4 invVP;
};

uniform sampler2D uColorTex;
uniform sampler2D uPositionTex;
uniform sampler2D uNormalTex;
uniform sampler2D uMetalRoughOccTex;
uniform sampler2D uDepthTex;

uniform sampler2D uSkyTex;

uniform vec3 uLightDirection;

out vec4 color;

const float PI = 3.145;

vec3 viewRay(mat4 invMat, vec3 coord) {
	vec3 ddz = invMat[2].xyz - invMat[2].w * coord.xyz;
	return -normalize(ddz);
}

const float gamma = 2.2;

float toGamma(float v) {
  return pow(v, 1.0 / gamma);
}

vec2 toGamma(vec2 v) {
  return pow(v, vec2(1.0 / gamma));
}

vec3 toGamma(vec3 v) {
  return pow(v, vec3(1.0 / gamma));
}

vec4 toGamma(vec4 v) {
  return vec4(toGamma(v.rgb), v.a);
}

void main()
{
    vec4 col = texture(uColorTex, vTexcoord.st);
    vec4 pos = texture(uPositionTex, vTexcoord.st);
    vec4 n = texture(uNormalTex, vTexcoord.st);
    vec4 mro = texture(uMetalRoughOccTex, vTexcoord.st);

    
    vec4 sp = (invVP * vec4(vTexcoord.st * 2.0 - 1.0, 0.0, 1.0));
    sp.xyz = sp.xyz / sp.w;


    vec3 eyeDir = viewRay(invVP, sp.xyz);
    vec2 bUV;
    bUV.s = 0.5 + atan(eyeDir.z, -eyeDir.x) / (2.0 * PI);
    bUV.t = 0.5 + asin(eyeDir.y) / PI;

    vec4 sky = texture(uSkyTex, bUV.st);

    sky.rgb = toGamma(sky.rgb);
    
    col.rgb = col.rgb * col.a + sky.rgb * (1.0 - col.a);


    float depth = texture(uDepthTex, vTexcoord.st).r;

    float d = max(dot(-uLightDirection, n.xyz), 0.0);



    if(n.w > 0.0 )
    {
        col.rgb *= d;
        col.rgb = col.rgb * 0.5 + col.rgb * mro.z;
    }


   

    color = col;

    gl_FragDepth = depth;
}