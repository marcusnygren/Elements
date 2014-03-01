#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec3 value;

layout (binding = 0) uniform sampler3D velocityTexture;
layout (binding = 1) uniform sampler3D temperatureTexture;
layout (binding = 2) uniform sampler3D densityTexture;

layout (location = 3) uniform float ambientTemperature;
layout (location = 4) uniform float timeStep;
layout (location = 5) uniform float alpha;
layout (location = 6) uniform float beta;

in float layer;

void main()
{
    ivec3 pos = ivec3(gl_FragCoord.xy, layer);
    float temp = texelFetch(temperatureTexture, pos, 0).r;
    vec3 velocity = texelFetch(velocityTexture, pos, 0).xyz;

    value = velocity;

    if (temp > ambientTemperature) {
        float density = texelFetch(densityTexture, pos, 0).x;
        value += (timeStep * (temp - ambientTemperature) * alpha - density * beta ) * vec3(0, -1, 0);
    }
}