attribute vec4 position;
attribute vec2 textureCoordinates;

varying vec2 varyingTextureCoordinates;

uniform vec2 modelPosition;

void main() {
	varyingTextureCoordinates = textureCoordinates;
	gl_Position = vec4(modelPosition.x, modelPosition.y, 0.0, 0.0) + position;
	
}