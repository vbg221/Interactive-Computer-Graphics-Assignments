/*
	Auther : Vatsal Gopani
	N ID: 17368916
	Subject : Interactive Computer Graphics
	Assignment 1 Question 2
*/


#include "glsupport.h"
#include <glut.h>
#include <Windows.h>
#include <gl\GL.h>
#include <glew.h>


GLuint  program;

GLuint VertexPosition1VBO;
GLuint TextureCoordinatesVBO;

GLuint positionAttribute1;
GLuint textureCoordinatesAttribute;

GLuint texturePicture;
GLuint texturePicture2;
GLuint positionUniform;


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition1VBO);
	glVertexAttribPointer(positionAttribute1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute1);

	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesVBO);
	glVertexAttribPointer(textureCoordinatesAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordinatesAttribute);


	glBindTexture(GL_TEXTURE_2D, texturePicture);
	glUniform2f(positionUniform, -0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, texturePicture2);
	glUniform2f(positionUniform, 0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(positionAttribute1);
	glDisableVertexAttribArray(textureCoordinatesAttribute);

	glutSwapBuffers();
}

void init() {
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");


	glUseProgram(program);
	positionAttribute1 = glGetAttribLocation(program, "position");
	textureCoordinatesAttribute = glGetAttribLocation(program, "textureCoordinates");

	texturePicture = loadGLTexture("The Cars 1Kx1K.png");
	texturePicture2 = loadGLTexture("The Cars 2 1Kx1K.png");

	glGenBuffers(1, &VertexPosition1VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition1VBO);

	GLfloat sqVerts[12] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f,

		-0.5f, -0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &TextureCoordinatesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesVBO);

	GLfloat sqTextCoords[12] = {

		/*   Effort of mapping triangles of texture image with the triangles of base shape coordinates
		NOTE : JPG Creates problem while rendering. USE png instead
		*/

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqTextCoords, GL_STATIC_DRAW);


}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}




void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CS-6533");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}