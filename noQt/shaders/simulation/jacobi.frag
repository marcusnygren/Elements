#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable


// out vec4 FragColor;

// layout (binding = 0) uniform sampler3D Pressure;
// layout (binding = 1) uniform sampler3D Divergence;
// layout (binding = 2) uniform sampler3D Obstacles;

// layout (location = 3) uniform float Alpha;
// layout (location = 4) uniform float InverseBeta;

// in float layer;

// void main()
// {
//     ivec3 T = ivec3(gl_FragCoord.xy, layer);

//     // Find neighboring pressure:
//     vec4 pN = texelFetchOffset(Pressure, T, 0, ivec3(0, 1, 0));
//     vec4 pS = texelFetchOffset(Pressure, T, 0, ivec3(0, -1, 0));
//     vec4 pE = texelFetchOffset(Pressure, T, 0, ivec3(1, 0, 0));
//     vec4 pW = texelFetchOffset(Pressure, T, 0, ivec3(-1, 0, 0));
//     vec4 pU = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, 1));
//     vec4 pD = texelFetchOffset(Pressure, T, 0, ivec3(0, 0, -1));
//     vec4 pC = texelFetch(Pressure, T, 0);

//     // Find neighboring obstacles:
//     vec3 oN = texelFetchOffset(Obstacles, T, 0, ivec3(0, 1, 0)).xyz;
//     vec3 oS = texelFetchOffset(Obstacles, T, 0, ivec3(0, -1, 0)).xyz;
//     vec3 oE = texelFetchOffset(Obstacles, T, 0, ivec3(1, 0, 0)).xyz;
//     vec3 oW = texelFetchOffset(Obstacles, T, 0, ivec3(-1, 0, 0)).xyz;
//     vec3 oU = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, 1)).xyz;
//     vec3 oD = texelFetchOffset(Obstacles, T, 0, ivec3(0, 0, -1)).xyz;

//     // Use center pressure for solid cells:
//     if (oN.x > 0) pN = pC;
//     if (oS.x > 0) pS = pC;
//     if (oE.x > 0) pE = pC;
//     if (oW.x > 0) pW = pC;
//     if (oU.x > 0) pU = pC;
//     if (oD.x > 0) pD = pC;

//     vec4 bC = texelFetch(Divergence, T, 0);
//     FragColor = (pW + pE + pS + pN + pU + pD + Alpha * bC) * 0.01f;
// }


out vec4 value;

layout (binding = 0) uniform sampler3D pressureTexture;
layout (binding = 1) uniform sampler3D bTexture; // Divergence
layout (binding = 2) uniform sampler3D obstacleTexture;  

layout (location = 3) uniform float alpha;
layout (location = 4) uniform float beta;

in float layer;

void main()
{
  ivec3 pos = ivec3(gl_FragCoord.xy, layer); // dela med 100??

  vec4 xUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(1, 0, 0));
  vec4 xDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(-1, 0, 0));
  vec4 yUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 1, 0));
  vec4 yDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, -1, 0));
  vec4 zUpP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, 1));
  vec4 zDownP = texelFetchOffset(pressureTexture, pos, 0, ivec3(0, 0, -1));
  vec4 centerP = texelFetch(pressureTexture, pos, 0);


  vec3 xUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(1, 0, 0)).xyz;
  vec3 xDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
  vec3 yUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 1, 0)).xyz;
  vec3 yDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, -1, 0)).xyz;
  vec3 zUpO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, 1)).xyz;
  vec3 zDownO = texelFetchOffset(obstacleTexture, pos, 0, ivec3(0, 0, -1)).xyz;

  if (xUpO.x > 0)
    xUpP = centerP;
  if (xDownO.x > 0)
    xDownP = centerP;

  if (yUpO.x > 0)
    yUpP = centerP;
  if (yDownO.x > 0)
    yDownP = centerP;

  if (zUpO.x > 0)
    zUpP = centerP;
  if (zDownO.x > 0)
    zDownP = centerP;

  vec4 bc = texelFetch(bTexture, pos, 0);

  value = (xUpP + xDownP + yUpP + yDownP + zUpP + zDownP + alpha * bc) * 0.01f;

  // Kanske approved!
}