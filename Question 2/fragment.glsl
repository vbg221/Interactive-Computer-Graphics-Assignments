varying vec2 varyingTextureCoordinates;
uniform sampler2D texture;



void main(){
	gl_FragColor = texture2D(texture, varyingTextureCoordinates);
}