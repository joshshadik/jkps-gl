#version 300 es
precision highp float;

in vec2 vTexcoord;

uniform sampler2D posTex;
uniform sampler2D velTex;

uniform float deltaTime;
uniform float currentTime;

uniform float lifetime;

uniform vec3 origin;

uniform vec3 direction;
uniform float magnitude;
uniform float randomness;

const vec3 GRAVITY = vec3(0.0, 0.0, 0.0);

layout(location = 0) out vec4 position;
layout(location = 1) out vec4 velocity;

highp float rand(highp vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec4 pos = texture(posTex, vTexcoord);
    vec4 vel = texture(velTex, vTexcoord);

    vec3 accel = GRAVITY;

    if( pos.w <= 0.0 )
    {
        vec3 r = vec3(rand(vec2(vTexcoord.x, currentTime)), rand(vec2(vTexcoord.y, currentTime)), rand(vec2(vTexcoord.x * ( 1.0 + vTexcoord.y), currentTime))) - vec3(0.5);
        accel += (direction * (1.0 - randomness) + r * randomness) * magnitude;
        pos.w = rand(vTexcoord * (1.0 + currentTime*currentTime)) * lifetime; // * 0.5 + lifetime * 0.75;
        pos.xyz = origin + r * randomness * 0.1;
        vel.xyz = r * randomness;
    }

    vel.xyz += accel * deltaTime;

    pos.xyz = pos.xyz + vel.xyz * deltaTime;
    pos.w = pos.w - deltaTime;

    position = pos;
    velocity = vel;
}