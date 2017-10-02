#version 330
in vec4 fragColor;
in vec4 fragGlobalColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec3 fragLightPositions[10];
flat in int fragNumberOfLights;
in vec3 fragGlobalLightPosition;
in vec3 fragCameraPosition;
flat in float fragGlobalLightIntensity;

out vec4 color;

uniform float materialAmbientIntensity;
uniform float materialShininess;
uniform vec4 materialSpecular;
uniform float lightIntensities[10];

void main()
{
	vec3 unitNormal = normalize(fragNormal);
	vec3 toCameraDir = normalize(fragCameraPosition - fragPosition);

	vec4 totalFragColor = vec4(0.0);
	vec4 totalSpecular = vec4(0.0);
	vec3 toLightDir;
	vec3 reflectDir;
	float phongTerm;
	float cosAngIncidence;
	for(int i = 0; i < fragNumberOfLights; i++){
		toLightDir = normalize(fragLightPositions[i] - fragPosition);
		reflectDir = reflect(-toLightDir, unitNormal);
		phongTerm = dot(toCameraDir, reflectDir);
		cosAngIncidence = dot(unitNormal, toLightDir);
		cosAngIncidence = clamp(cosAngIncidence, 0.0, 1.0);

		phongTerm = clamp(phongTerm, 0.0, 1.0);
		phongTerm = cosAngIncidence != 0.0 ? phongTerm : 0.0;
		phongTerm = pow(phongTerm, materialShininess);

		totalFragColor += (fragColor * lightIntensities[i] * cosAngIncidence);
		totalSpecular += (materialSpecular * phongTerm * lightIntensities[i] );
	}

	vec3 toGlobalLightDir = normalize(fragGlobalLightPosition - fragPosition);
	vec3 globalReflectDir = reflect(-toGlobalLightDir, unitNormal);
	float globalPhongTerm = dot(toCameraDir, globalReflectDir);

	float globalCosAngIncidence = dot(unitNormal, toGlobalLightDir);
	globalCosAngIncidence = clamp(globalCosAngIncidence, 0.0, 1.0);

	globalPhongTerm = clamp(globalPhongTerm, 0.0, 1.0);
	globalPhongTerm = globalCosAngIncidence != 0.0 ? globalPhongTerm : 0.0;
	globalPhongTerm = pow(globalPhongTerm, materialShininess);

	color = totalFragColor 
		+ fragGlobalColor 
		+ (materialSpecular * globalPhongTerm * fragGlobalLightIntensity) 
		+ totalSpecular
		+ (fragColor * materialAmbientIntensity);
}