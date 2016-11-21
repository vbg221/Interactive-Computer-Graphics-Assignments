varying vec3 varyingNormal;
varying vec3 varyingPosition;

uniform vec3 specularLightColor;

struct Light{
	vec3 lightDirection;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[2];

float attenuate(float dist, float a, float b) {
	return 1.0 / (1.0 + a*dist + b*dist*dist);
}

uniform vec3 uColor = vec3(1.0, 1.2, 1.0);

void main() {
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	
	for(int i=0; i<2; i++) {
		float diffuse = max(0.1, dot(varyingNormal, lights[i].lightDirection));
		diffuseColor += lights[i].lightColor * diffuse;
	}
	for(int i=0; i<2; i++) {
		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lights[i].lightDirection);
		float specular = pow(max(0.0, dot(h, varyingNormal)), 64.0);
		specularColor += lights[i].specularLightColor * specular ;
	}

	vec3 intensity = (uColor * diffuseColor) + specularColor;
	gl_FragColor = vec4(intensity.xyz, 1.0);
}
