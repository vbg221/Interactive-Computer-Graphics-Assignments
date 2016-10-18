varying vec4 varyingNormal;

uniform vec3 uColor = vec3(1.0, 1.2, 0.0);

void main(){
	float diffuse = max(0.1, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	vec3 intensity = uColor * diffuse;
	gl_FragColor = vec4(intensity.xyz, 1.0);
}
