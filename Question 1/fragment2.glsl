varying vec2 varyingTextureCoordinates2;
uniform sampler2D texture2;

void main(){
	gl_FragColor = texture2D(texture2, varyingTextureCoordinates2);
}