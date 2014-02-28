#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec3 FragColor;

layout (binding = 0) uniform sampler3D Velocity;
layout (binding = 1) uniform sampler3D Pressure;
layout (binding = 2) uniform sampler3D Obstacles;
layout (location = 3) uniform float GradientScale;

in float layer;

void main()
{
    ivec3 T = ivec3(gl_FragCoord.xy, layer);

    vec3 oC = texelFetch(Obstacles, T, 0).xyz;
    if (oC.x > 0) {
        FragColor = oC.yzx;
        return;
    }

    // Find neighboring pressure:
    float pN = texelFetchOffset(Pressure, T, 0, ivec3(0, 1, 0)).r;
    float pS = texelFetchOffset(Pressure, T, 0, ivec3(0, -1, 0)).r;
    float pE = texelFetchOffset(Pressure, T, 0, ivec3(1, 0, 0)).r;
    float pW = texelFetchOffset(Pressure, T, 0, ivec3(-1, 0, 0)).r;
    float pU = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, 1)).r;
    float pD = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, -1)).r;
    float pC = texelFetch(Pressure, T, 0).r;

    // Find neighboring obstacles:
    vec3 oN = texelFetchOffset(Obstacles, T, 0, ivec3(0, 1, 0)).xyz;
    vec3 oS = texelFetchOffset(Obstacles, T, 0, ivec3(0, -1, 0)).xyz;
    vec3 oE = texelFetchOffset(Obstacles, T, 0, ivec3(1, 0, 0)).xyz;
    vec3 oW = texelFetchOffset(Obstacles, T, 0, ivec3(-1, 0, 0)).xyz;
    vec3 oU = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, 1)).xyz;
    vec3 oD = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, -1)).xyz;

    // Use center pressure for solid cells:
    vec3 obstV = vec3(0);
    vec3 vMask = vec3(1);

    if (oN.x > 0) { pN = pC; obstV.y = oN.z; vMask.y = 0; }
    if (oS.x > 0) { pS = pC; obstV.y = oS.z; vMask.y = 0; }
    if (oE.x > 0) { pE = pC; obstV.x = oE.y; vMask.x = 0; }
    if (oW.x > 0) { pW = pC; obstV.x = oW.y; vMask.x = 0; }
    if (oU.x > 0) { pU = pC; obstV.z = oU.x; vMask.z = 0; }
    if (oD.x > 0) { pD = pC; obstV.z = oD.x; vMask.z = 0; }

    // Enforce the free-slip boundary condition:
    vec3 oldV = texelFetch(Velocity, T, 0).xyz;
    vec3 grad = vec3(pE - pW, pN - pS, pU - pD) * GradientScale;
    vec3 newV = oldV - grad;
    FragColor = (vMask * newV) + obstV;  
}










// out vec3 value;

// layout (binding = 0) uniform sampler3D velocityTexture;
// layout (binding = 1) uniform sampler3D pressureTexture;
// layout (binding = 2) uniform sampler3D obstacleTexture;

// layout (location = 3) uniform float gridScale;

// in float layer;

// void main()
// {
//   ivec3 pos = ivec3(gl_FragCoord.xy, layer); // dela med 100?

//   vec3 solid = texelFetch(obstacleTexture, pos, 0).xyz;
//   if (solid.x > 0)
//   {
//     value = solid.yzx;
//     return;
//   }

//   float xUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(1, 0, 0)).x;
//   float xDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(-1, 0, 0)).x;
//   float yUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 1, 0)).x;
//   float yDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, -1, 0)).x;
//   float zUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, 1)).x;
//   float zDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, -1)).x;
//   float centerP = texelFetch(pressureTexture, pos, 0).x;


//   vec3 xUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(1, 0, 0)).xyz;
//   vec3 xDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
//   vec3 yUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 1, 0)).xyz;
//   vec3 yDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, -1, 0)).xyz;
//   vec3 zUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;
//   vec3 zDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, -1)).xyz;

//   vec3 obstacleVelocity = vec3(0);
//   vec3 mask = vec3(1);

//   if (xUpO.x > 0)
//   {
//     xUpP = centerP;
//     obstacleVelocity.x = xUpO.y;
//     mask.x = 0;
//   }
//   if (xDownO.x > 0)
//   {
//     xDownP = centerP;
//     obstacleVelocity.x = xDownO.y;
//     mask.x = 0;
//   }

//   if (yUpO.x > 0)
//   {
//     yUpP = centerP;
//     obstacleVelocity.y = yUpO.z;
//     mask.y = 0;
//   }
//   if (yDownO.x > 0)
//   {
//     yDownP = centerP;
//     obstacleVelocity.y = yDownO.z;
//     mask.y = 0;
//   }

//   if (zUpO.x > 0)
//   {
//     zUpP = centerP;
//     obstacleVelocity.z = xUpO.x;
//     mask.z = 0;
//   }
//   if (zDownO.x > 0)
//   {
//     zDownP = centerP;
//     obstacleVelocity.z = xDownO.x;
//     mask.z = 0;
//   }

//   vec3 velocity = texelFetch(velocityTexture, pos, 0).xyz;
//   vec3 pressureGradient = vec3(xUpP - xDownP, yUpP - yDownP, zUpP - zDownP) * 0.5 * gridScale;
//   vec3 subtractedVelocity = velocity - pressureGradient;
//   value = (mask * subtractedVelocity) + obstacleVelocity;

// }