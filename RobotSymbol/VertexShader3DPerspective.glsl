#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColor;
out vec4 fragColor;

uniform mat4 cameraToScreenMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 localToWorldMatrix;

void main()
{
	gl_Position = cameraToScreenMatrix * worldToCameraMatrix * localToWorldMatrix * position;
	fragColor = vertexColor;
}