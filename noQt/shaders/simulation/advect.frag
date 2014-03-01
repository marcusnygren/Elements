#version 330

out vec4 value;

uniform sampler3D sourceTexture;
uniform sampler3D velocityTexture;
uniform sampler3D obstacleTexture;

uniform float timeStep;
uniform vec3 dxyz;

in float layer;

void main(void)
{
  vec3 fragPos = vec3(gl_FragCoord.xy, layer);
  float inObstacle = texture(obstacleTexture, fragPos * dxyz).x;
  if (inObstacle > 0)
  {
    value = vec4(0);
    return;
  }
  // value = vec4(0,1,0,1);
  vec3 velocity = texture(velocityTexture, fragPos * dxyz).xyz;
  vec3 pos = (fragPos - velocity * timeStep);
  value = texture(sourceTexture, pos * dxyz);


  // Approved
}
