#version 300 es
precision highp float;

in vec2 vTexcoord;

uniform sampler2D posTex;
uniform sampler2D velTex;

uniform float deltaTime;
uniform float currentTime;

uniform vec3 direction;
uniform float magnitude;
uniform float randomness;

const float GRAVITY = -10.0;

layout(location = 0) out vec4 velocity;

highp float rand(highp vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec4 pos = texture(posTex, vTexcoord);
    vec4 vel = texture(velTex, vTexcoord);

    vec3 accel = GRAVITY;

    if( pos.w <= -1.0 )
    {
        vec3 r = vec3(rand(vTexcoord.x, currentTime), rand(vTexcoord.y, currentTime), rand(vTexcoord.x * ( 1 + vTexcoord.y), currentTime));
        accel += (direction * (1.0 - randomness) + r * randomness) * magnitude;
    }

    vel.xyz += accel * deltaTime;
    
    velocity = vel;
}