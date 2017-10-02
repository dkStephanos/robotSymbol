#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;

out vec4 fragColor;
out vec4 fragGlobalColor;
out vec3 fragNormal;
out vec3 fragPosition;
out vec3 fragLightPositions[10];
flat out int fragNumberOfLights;
out vec3 fragGlobalLightPosition;
out vec3 fragCameraPosition;
flat out float fragGlobalLightIntensity;

uniform vec3 worldGlobalLightPos;
uniform float globalLightIntensity;
uniform vec3 lightPositions[10];
uniform int numberOfLights;
uniform vec3 worldCameraPos;
uniform mat4 cameraToScreenMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 localToWorldMatrix;

void main()
{
	vec4 positionInWorld = localToWorldMatrix * position;
	gl_Position = cameraToScreenMatrix * worldToCameraMatrix * positionInWorld;

	vec3 worldVertexNormal = mat3(localToWorldMatrix) * vertexNormal;
	fragNormal = vec3(worldVertexNormal);
	vec3 unitNormal = normalize(fragNormal);
	vec3 toLightDir = normalize(worldGlobalLightPos);
	float cosAngIncidence = dot(unitNormal, toLightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0.0f, 1.0f);

	fragGlobalLightIntensity = globalLightIntensity;
	fragLightPositions = lightPositions;
	fragNumberOfLights = numberOfLights;
	fragGlobalColor = vertexColor * globalLightIntensity * cosAngIncidence;
	fragPosition = vec3(positionInWorld);
	fragGlobalLightPosition = worldGlobalLightPos;
	fragCameraPosition = worldCameraPos;
	fragColor = vertexColor;
}