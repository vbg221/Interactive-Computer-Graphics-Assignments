attribute vec4 position;
attribute vec2 textureCoordinates;

varying vec2 varyingTextureCoordinates;

void main() {
	gl_Position = position;
	varyingTextureCoordinates = textureCoordinates;
}