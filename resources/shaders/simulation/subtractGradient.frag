#version 330 core

out vec3 value;

layout (binding = 0) uniform sampler3D velocityTexture;
layout (binding = 1) uniform sampler3D pressureTexture;
layout (binding = 2) uniform sampler3D obstacleTexture;

layout (location = 0) uniform int layer;
layout (location = 1) uniform float gridScale;

void main()
{
  ivec3 pos = ivec3(gl_FragCoord.xy, layer);

  vec3 solid = texelFetch(obstacleTexture, pos, 0);
  if (solid.x > 0)
  {
    value = solid.yzx;
    return;
  }

  float xUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(1, 0, 0)).x;
  float xDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(-1, 0, 0)).x;
  float yUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 1, 0)).x;
  float yDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, -1, 0)).x;
  float zUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, 1)).x;
  float zDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, 1)).x;
  float centerP = texelFetch(pressureTexture, pos, 0);


  vec3 xUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(1, 0, 0)).xyz;
  vec3 xDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
  vec3 yUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 1, 0)).xyz;
  vec3 yDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, -1, 0)).xyz;
  vec3 zUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;
  vec3 zDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;

  vec3 obstacleVelocity = vec3(0);
  vec3 mask = vec3(1);

  if (xUpO.x > 0)
  {
    xUpP = centerP;
    obstacleVelocity.x = xUpO.y;
    mask.x = 0;
  }
  if (xDownO.x > 0)
  {
    xDownP = centerP;
    obstacleVelocity.x = xDownO.y;
    mask.x = 0;
  }

  if (yUpO.x > 0)
  {
    yUpP = centerP;
    obstacleVelocity.y = yUpO.z;
    mask.y = 0;
  }
  if (yDownO.x > 0)
  {
    yDownP = centerP;
    obstacleVelocity.y = yDownO.z;
    mask.y = 0;
  }

  if (zUpO.x > 0)
  {
    zUpP = centerP;
    obstacleVelocity.z = xUpO.x;
    mask.z = 0;
  }
  if (zDownO.x > 0)
  {
    zDownP = centerP;
    obstacleVelocity.z = xDownO.x;
    mask.z = 0;
  }

  vec3 velocity = texelFetch(velocityTexture, pos, 0);
  vec3 pressureGradient = vec3(xUpP - xDownP, yUpP - yDownP, zUpP - zDownP) * 0.5 * gridScale;
  vec3 subtractedVelocity = velocity - pressureGradient;
  value = (mask * subtractedVelocity) + obstacleVelocity;

}