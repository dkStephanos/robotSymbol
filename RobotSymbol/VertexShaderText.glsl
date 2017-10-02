#version 330
layout(location = 0) in vec4 posTexCoord;

out vec2 fragTexCoord;

void main()
{
	// The texture coord is the last 2
	fragTexCoord = posTexCoord.zw;
	// The position is the first 2
	gl_Position = vec4(posTexCoord.x, posTexCoord.y, 0, 1);
}