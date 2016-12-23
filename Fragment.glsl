varying vec3 varyingNormal;
varying vec3 varyingPosition;
varying vec2 varyingTexCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

varying mat3 varyingTBNMatrix;

struct Light{
	vec3 lightPosition;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[2];

// Attenuation is not working. Need to work on that. Until then, work with Direction Lights.


//uniform vec3 uColor = vec3(1.0, 1.2, 1.0);

void main() {
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	
	vec3 textureNormal = normalize((texture2D(normalTexture, varyingTexCoord).xyz * 2.0) - 1.0);
	textureNormal = normalize(varyingTBNMatrix * textureNormal);

	for(int i=0; i<2; i++) {
		vec3 lightDirection = -normalize(varyingPosition - lights[i].lightPosition);
		float diffuse = max(0.0, dot(varyingNormal, lightDirection));

		diffuseColor += (lights[i].lightColor * diffuse) ;


		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lights[i].lightPosition);
		float specular = pow(max(0.0, dot(h, varyingNormal)), 64.0);
		specularColor += lights[i].specularLightColor * specular ;
	}

	vec3 intensity = (texture2D(diffuseTexture, varyingTexCoord).xyz * diffuseColor) + ((texture2D(specularTexture, varyingTexCoord).x) * specularColor);
	
//	gl_FragColor = texture2D(diffuseTexture, varyingTexCoord);
	gl_FragColor = vec4(intensity.xyz, 1.0);
}
