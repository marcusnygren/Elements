#version 330 core

out vec4 value;

layout (binding = 0) uniform sampler3D sourceTexture;
layout (binding = 1) uniform sampler3D velocityTexture;
layout (binding = 2) uniform sampler3D obstacleTexture;

layout (location = 3) uniform float timeStep;
layout (location = 4) uniform vec3 dxyz;

flat in int layer;

void main(void)
{
  vec3 fragPos = vec3(gl_FragCoord.xy, layer);
  float inObstacle = texture(obstacleTexture, fragPos * dxyz).x;
  if (inObstacle > 0)
  {
    value = vec4(0);
    return;
  }

  vec3 velocity = texture(velocityTexture, fragPos * dxyz).xyz;
  vec3 pos = dxyz * (fragPos - velocity * timeStep);
  value = texture(sourceTexture, pos);
}