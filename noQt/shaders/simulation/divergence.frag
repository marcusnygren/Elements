#version 330 core

out float value;

layout (binding = 0) uniform sampler3D velocityTexture;
layout (binding = 1) uniform sampler3D obstacleTexture;

layout (location = 2) uniform float cellSize;

flat in int layer;

void main()
{
  ivec3 pos = ivec3(gl_FragCoord.xy, layer);

  vec3 xUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(1, 0, 0)).xyz;
  vec3 xDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
  vec3 yUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 1, 0)).xyz;
  vec3 yDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, -1, 0)).xyz;
  vec3 zUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, 1)).xyz;
  vec3 zDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, 1)).xyz;

  vec3 xUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(1, 0, 0)).xyz;
  vec3 xDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
  vec3 yUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 1, 0)).xyz;
  vec3 yDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, -1, 0)).xyz;
  vec3 zUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;
  vec3 zDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;

  if (xUpO.x > 0)
    xUpV = xUpO.yzx;
  if (xDownO.x > 0)
    xDownV = xDownO.yzx;

  if (yUpO.x > 0)
    yUpV = yUpO.yzx;
  if (yDownO.x > 0)
    yDownV = yDownO.yzx;

  if (zUpO.x > 0)
    zUpV = zUpO.yzx;
  if (xDownO.z > 0)
    zDownV = zDownO.yzx;

    value = (xUpV.x - xDownV.x + yUpV.y - yDownV.y + zUpV.z - zDownV.z) / (2 * cellSize);
}