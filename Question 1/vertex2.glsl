attribute vec4 position2;
attribute vec2 textureCoordinates2;

varying vec2 varyingTextureCoordinates2;

void main() {
	gl_Position = position2;
	varyingTextureCoordinates2 = textureCoordinates2;
}