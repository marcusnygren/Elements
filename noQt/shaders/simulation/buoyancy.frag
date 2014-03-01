#version 330

out vec3 value;

uniform sampler3D velocityTexture;
uniform sampler3D temperatureTexture;
uniform sampler3D densityTexture;

uniform float ambientTemperature;
uniform float timeStep;
uniform float alpha;
uniform float beta;

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