#version 330

in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texFragColor = texture(tex, fragTexCoord);
	texFragColor.a = 0.5;
	if(texFragColor.r == 0 && texFragColor.g == 0 && texFragColor.b == 0){
		discard;
	}else{
		color = texFragColor;
	}
	
}