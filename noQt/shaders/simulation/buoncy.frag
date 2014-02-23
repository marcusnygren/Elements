#version 330 core

out vec3 value;
layout (binding = 0) uniform sampler3D velocityTexture;
layout (binding = 1) uniform sampler3D temperatureTexture;
layout (binding = 2) uniform sampler3D densityTexture;

layout (binding = 3) uniform float startTemperature;
layout (binding = 4) uniform float timeStep;
layout (binding = 5) uniform float alpha;
layout (binding = 6) uniform float beta;
layout (binding = 7) uniform float gridScale;

in int layer;

void main(void)
{
	vec3 pos = vec3(gl_FragCoord.xy, layer);
	vec3 yUp = vec3(0, 1, 0);
	
	// texelFetch reutrn a non-normalized vec4(R,0,0,1) for R32F
	// and 	normalized vec4(R,G,B,A) for RGBA16
	float temperature = texelFetch(temperatureTexture, pos, 0).r;
	float density = texelFetch(densityTexture, pos, 0).x;

	vec3 velocity = texelFetch(velocityTexture, pos, 0).xyz;
	
	if(temperature > startTemperature)
	{
		FragColor = velocity + yUp*(-density*alpha + timeStep*(temperature - startTemperature)*beta;
	}
	else
	{
		FragColor = velocity;
	}
	
	// Calculate the newTemperature & newDensity
	/*
	
	vec3 pos = vec3(gl_FragCoord.xy, layer);
	vec3 yUp = vec3(0, 1, 0);
	
	float temperature = texelFetch(temperatureTexture, pos, 0).r;
	float density = texelFetch(densityTexture, pos, 0).x;
	vec3 velocity = texelFetch(velocityTexture, pos, 0).xyz;
	
	// This (newTemperature & newDensity) should maybe be calculated somewhere else.
	// Start of: Calculate the new temperature and the new density, which belong to next frame
	float xUpT = texelFetch(temperatureTexture, pos, 0, vec3(1, 0, 0)).x;
	float xDownT = texelFetchOffset(temperatureTexture, pos, 0, ivec3(-1, 0, 0)).x;
	float yUpT = texelFetchOffset(temperatureTexture, pos, 0, ivec3(0, 1, 0)).x;
	float yDownT = texelFetchOffset(temperatureTexture, pos, 0, ivec3(0, -1, 0)).x;
	float zUpT = texelFetchOffset(temperatureTexture, pos, 0, ivec3(0, 0, 1)).x;
	float zDownT = texelFetchOffset(temperatureTexture, pos, 0, ivec3(0, 0, 1)).x;
	float centerT = texelFetch(temperatureTexture, pos, 0);

	float xUpD = texelFetch(densityTexture, pos, 0, vec3(1, 0, 0)).x;
	float xDownD = texelFetchOffset(densityTexture, pos, 0, ivec3(-1, 0, 0)).x;
	float yUpD = texelFetchOffset(densityTexture, pos, 0, ivec3(0, 1, 0)).x;
	float yDownD = texelFetchOffset(densityTexture, pos, 0, ivec3(0, -1, 0)).x;
	float zUpD = texelFetchOffset(densityTexture, pos, 0, ivec3(0, 0, 1)).x;
	float zDownD = texelFetchOffset(densityTexture, pos, 0, ivec3(0, 0, 1)).x;
	float centerD = texelFetch(densityTexture, pos, 0);

	float newTemperature = -(velocity.x * (xUpT - xDownT) + velocity.y * (yUpT - yDownT) + velocity.z * (zUpT - zDownT))*gridScale*timeStep;
	float newDensity = -(velocity.x * (xUpD - xDownD) + velocity.y * (yUpD - yDownD) + velocity.z * (zUpD - zDownD))*gridScale*timeStep;
	// End of: Calculating newTemperature & newDensity

	if(temperature > startTemperature)
	{
		// Add the buoncy force
		FragColor = velocity + (-alpha*density*yUp) + beta*(temperature-startTemperature)*yUp;
	}
	else
	{
		FragColor = velocity + (-alpha*density*yUp);
	}

	
	*/
}
