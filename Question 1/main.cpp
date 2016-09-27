/*
	Auther : Vatsal Gopani
	N ID: 17368916
	Subject : Interactive Computer Graphics
	Assignment 1 Question 1
*/


#include "glsupport.h"
#include <glut.h>
#include <Windows.h>
#include <gl\GL.h>
#include <glew.h>


GLuint program;
GLuint program2;

GLuint VertexPosition1VBO;
GLuint VertexPosition2VBO;
GLuint TextureCoordinatesVBO;
GLuint TextureCoordinates2VBO;

GLuint positionAttribute1;
GLuint positionAttribute2;
GLuint textureCoordinatesAttribute;
GLuint textureCoordinatesAttribute2;

GLuint texturePicture;
GLuint texturePicture2;
GLuint positionUniform;


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);										

	glActiveTexture(texturePicture);
	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition1VBO);
	glVertexAttribPointer(positionAttribute1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute1);

	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesVBO);
	glVertexAttribPointer(textureCoordinatesAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordinatesAttribute);


	glBindTexture(GL_TEXTURE_2D, texturePicture);
	glDrawArrays(GL_TRIANGLES, 0, 12);


	

	glUseProgram(program2);
	
	glActiveTexture(texturePicture2);
	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition2VBO);
	glVertexAttribPointer(positionAttribute2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinates2VBO);
	glVertexAttribPointer(textureCoordinatesAttribute2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordinatesAttribute2);
	
	glBindTexture(GL_TEXTURE_2D, texturePicture2);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	
	glDisableVertexAttribArray(positionAttribute1);
	glDisableVertexAttribArray(textureCoordinatesAttribute);
	glDisableVertexAttribArray(positionAttribute2);
	glDisableVertexAttribArray(textureCoordinatesAttribute2);

    glutSwapBuffers();
}

void init() {
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");

	
	glUseProgram(program);
	positionAttribute1 = glGetAttribLocation(program, "position");
	textureCoordinatesAttribute = glGetAttribLocation(program, "textureCoordinates");

	texturePicture = loadGLTexture("The Cars 1Kx1K.png");

	glGenBuffers(1, &VertexPosition1VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition1VBO);
		
	GLfloat sqVerts[24] = {
		0.0f, 0.0f,
		-1.0f, 0.0f,
		-1.0f, 1.0f,
		
		0.0f, 0.0f,
		-1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f

	};
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &TextureCoordinatesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesVBO);

	GLfloat sqTextCoords[24] = {

		/*   Effort of mapping triangles of texture image with the triangles of base shape coordinates   
			NOTE : JPG Creates problem while rendering. USE png instead
		*/
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f

		

	};
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqTextCoords, GL_STATIC_DRAW);

	program2 = glCreateProgram();
	readAndCompileShader(program2, "vertex2.glsl", "fragment2.glsl");

	glUseProgram(program2);
	positionAttribute2 = glGetAttribLocation(program2, "position2");
	textureCoordinatesAttribute2 = glGetAttribLocation(program2, "textureCoordinates2");

	texturePicture2 = loadGLTexture("The Cars 2 1Kx1K.png");

	glGenBuffers(1, &VertexPosition2VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexPosition2VBO);

	GLfloat sqVerts2[24]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, -1.0f,

		0.0f, 0.0f,
		1.0f, -1.0f,
		0.0f, -1.0f,

		0.0f, 0.0f,
		0.0f, -1.0f,
		-1.0f, -1.0f,

		0.0f, 0.0f,
		-1.0f, -1.0f,
		-1.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqVerts2, GL_STATIC_DRAW);

	glGenBuffers(1, &TextureCoordinates2VBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinates2VBO);

	GLfloat sqTextCoords2[24] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), sqTextCoords2, GL_STATIC_DRAW);
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