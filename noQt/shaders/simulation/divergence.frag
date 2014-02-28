#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable



out float FragColor;

layout(binding = 0) uniform sampler3D Velocity;
layout(binding = 1) uniform sampler3D Obstacles;
layout(location = 2) uniform float HalfInverseCellSize;

in float layer;

void main()
{
    ivec3 T = ivec3(gl_FragCoord.xy, layer);

    // Find neighboring velocities:
    vec3 vN = texelFetchOffset(Velocity, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 vS = texelFetchOffset(Velocity, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 vE = texelFetchOffset(Velocity, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 vW = texelFetchOffset(Velocity, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 vU = texelFetchOffset(Velocity, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 vD = texelFetchOffset(Velocity, T, 0, ivec3(0, 0, -1)).xyz;

    // Find neighboring obstacles:
    vec3 oN = texelFetchOffset(Obstacles, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 oS = texelFetchOffset(Obstacles, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 oE = texelFetchOffset(Obstacles, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 oW = texelFetchOffset(Obstacles, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 oU = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 oD = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, -1)).xyz;

    // Use obstacle velocities for solid cells:
    if (oN.x > 0) vN = oN.yzx;
    if (oS.x > 0) vS = oS.yzx;
    if (oE.x > 0) vE = oE.yzx;
    if (oW.x > 0) vW = oW.yzx;
    if (oU.x > 0) vU = oU.yzx;
    if (oD.x > 0) vD = oD.yzx;

    FragColor = HalfInverseCellSize * (vE.x - vW.x + vN.y - vS.y + vU.z - vD.z);
}



// out float value;

// layout(binding = 0) uniform sampler3D velocityTexture;
// layout(binding = 1) uniform sampler3D obstacleTexture;

// layout(location = 2) uniform float cellSize;

// in float layer;

// void main()
// {
//   ivec3 pos = ivec3(gl_FragCoord.xy, layer);

//   vec3 xUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(1, 0, 0)).xyz;
//   vec3 xDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
//   vec3 yUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 1, 0)).xyz;
//   vec3 yDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, -1, 0)).xyz;
//   vec3 zUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, 1)).xyz;
//   vec3 zDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, -1)).xyz;

//   vec3 xUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(1, 0, 0)).xyz;
//   vec3 xDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
//   vec3 yUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 1, 0)).xyz;
//   vec3 yDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, -1, 0)).xyz;
//   vec3 zUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;
//   vec3 zDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, -1)).xyz;

//   if (xUpO.x > 0)
//     xUpV = xUpO.yzx;
//   if (xDownO.x > 0)
//     xDownV = xDownO.yzx;

//   if (yUpO.x > 0)
//     yUpV = yUpO.yzx;
//   if (yDownO.x > 0)
//     yDownV = yDownO.yzx;

//   if (zUpO.x > 0)
//     zUpV = zUpO.yzx;
//   if (xDownO.z > 0)
//     zDownV = zDownO.yzx;

//     value = (xUpV.x - xDownV.x + yUpV.y - yDownV.y + zUpV.z - zDownV.z) / (2 * cellSize);
// }