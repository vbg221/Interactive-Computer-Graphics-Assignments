/*
	Auther : Vatsal Gopani
	N ID: 17368916
	Subject : Interactive Computer Graphics
	Assignment 1 Bonus Question
*/


#include "glsupport.h"
#include <glut.h>
#include <iostream>

using namespace std;

GLint program;
GLuint vertPostionVBO;
GLuint positionAttribute;

GLuint vertTexCoordVBO;
GLuint textureCoordAttribute;

GLuint timerUniform;
GLuint positionUniform;
float offset = 0.0;

GLuint Texture;


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);


	glUniform1f(timerUniform, offset);

	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);

	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	glVertexAttribPointer(textureCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordAttribute);

	
	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);



	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(textureCoordAttribute);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		offset += 0.02;
		break;
	case 'd':
		offset -= 0.02;
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	float newPositionX = (float)x / 250.0f - 1.0f;
	float newPositionY = (1.0 - (float)y / 250.0);
	glUniform2f(positionUniform, newPositionX, newPositionY);

}

void mouseMove(int x, int y) {

	float newPositionX = (float)x / 250.0f - 1.0f;
	float newPositionY = (1.0 - (float)y / 250.0);
	glUniform2f(positionUniform, newPositionX, newPositionY);

}

void init() {
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");
	Texture = loadGLTexture("The Cars 2 1Kx1K.png");

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	textureCoordAttribute = glGetAttribLocation(program, "textureCoordinates");
	timerUniform = glGetUniformLocation(program, "time");
	positionUniform = glGetUniformLocation(program, "modelPosition");

	glGenBuffers(1, &vertPostionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);

	GLfloat sqVert[12] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,

		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqVert, GL_STATIC_DRAW);

	glGenBuffers(1, &vertTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
	GLfloat sqTexCoords[12] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqTexCoords, GL_STATIC_DRAW);

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
	glutCreateWindow("CS - 6533");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	init();
	glutMainLoop();
	return 0;
}