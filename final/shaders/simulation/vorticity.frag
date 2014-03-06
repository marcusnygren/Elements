#version 330 core

out float value;

uniform sampler3D velocityTexture;

uniform float gridScale;

in float layer;


void main()
{
  ivec3 pos = ivec3(gl_FragCoord.xy, layer);

  vec3 xUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(1, 0, 0)).xyz;
  vec3 xDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(-1, 0, 0)).xyz;
  vec3 yUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 1, 0)).xyz;
  vec3 yDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, -1, 0)).xyz;
  vec3 zUpV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, 1)).xyz;
  vec3 zDownV = texelFetchOffset(velocityTexture, pos, 0, ivec3(0, 0, 1)).xyz;

  value = (gridScale * ((xUpV.y - xDownV.y) - (yUpV.z - yDownV.z)) - ((zUpV.x -zDownV.x) - (xUpV.z - xDownV.z)) - ((yUpV.x - yDownV.x) - (xUpV.y - xDownV.y)));
  
}

#version 330 core

out vec4 value;

uniform sampler3D velocityTexture;
uniform sampler3D vorticityTexture;

uniform vec3 dxScale;

uniform float timeStep;
uniform float gridScale; // t.ex. gridscale = 0.01
in float layer;


void main()
{
  ivec3 pos = ivec3(gl_FragCoord.xy, layer);

  float xUpV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(1, 0, 0)).x;
  float xDownV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(-1, 0, 0)).x;
  float yUpV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(0, 1, 0)).x;
  float yDownV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(0, -1, 0)).x;
  float zUpV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(0, 0, 1)).x;
  float zDownV = texelFetchOffset(vorticityTexture, pos, 0, ivec3(0, 0, 1)).x;
  float centerVort = texelFetchOffset(vorticityTexture, pos, 0).x;
  
 
  // Derivative 
  vec3 force = gridScale * 0.5 * vec3(abs(xUpV) - abs(xDownV), abs(yUpV) - abs(yDownV), abs(zUpV) - abs(zDownV));
  
  // Normalization
  const float EPSILON = 0.00000024144;
  float notZero = max(EPSILON, dot(force,force));
  vec3 normForce = force * inversesqrt(notZero);
  
  // Cross-product
  vec3 finalForce =  centerVort * (normForce * vec3(1, -1, 1)) * dxScale;
  
  vec3 velocity = texelFetchOffset(velocityTexture, pos, 0).xyz;

  value = velocity + timeStep * finalForce;
  
}




