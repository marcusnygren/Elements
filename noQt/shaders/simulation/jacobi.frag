#version 330

out vec4 value;

uniform sampler3D pressureTexture;
uniform sampler3D bTexture; // Divergence
uniform sampler3D obstacleTexture;  

uniform float alpha;
uniform float beta;

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