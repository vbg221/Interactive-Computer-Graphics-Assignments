varying vec2 varyingTextureCoordinates;
uniform sampler2D texture;

uniform float time;

void main(){
	vec2 textureCoordinates = vec2(varyingTextureCoordinates.x + time, varyingTextureCoordinates.y);
	gl_FragColor = texture2D(texture, textureCoordinates);
}