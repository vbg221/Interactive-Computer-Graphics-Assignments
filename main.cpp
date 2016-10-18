#include "glsupport.h"
#include "glut.h"
#include "iostream"
#include "matrix4.h"
#include "quat.h"

using namespace std;

GLuint program;
GLuint vertPostionVBO;
GLuint positionAttribute;
GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint normalPositionVBO;
GLuint normalAttribute;
GLuint normalMatrixUniformLocation;


class Entity {
public:

	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Matrix4 modelMatrix;
	Entity *parent;


	/*	This constructer sets the Object scale property to 1.0 by default.

		Arguments	: None (Default Constructer)

		Output		: Sets Scaling property of Object to 1.0.
	*/
	Entity(void) {
		s = {1.0, 1.0, 1.0};
	}


	/*	This constructer initializes the object by setting up its transformation coordinates.

		Arguments	: Takes 3 Cvec3 type vectors for Translation, Rotation and Scaling respectively.

		Output		: Sets Translation, Rotation and Scaling properties respectively.
	*/
	Entity(Cvec3 tr, Cvec3 ro, Cvec3 sc) {
		t = tr;
		r = ro;
		s = sc;
	}



	/*	This function applies transformation to the object. (Independent translation, rotation and scaling)
		
		Arguments	: Takes floating point values to determine whether it changes with time or not. (1 in case it does not change with time.)

		Output		: Transformed Model Matrix. 	
	*/
	void Transformation(float t1, float t2, float t3) {
		Quat Qx = Quat::makeXRotation(r[0] * t1);
		Quat Qy = Quat::makeYRotation(r[1] * t2);
		Quat Qz = Quat::makeZRotation(r[2] * t3);
		Matrix4 rotation = quatToMatrix(Qx * Qy * Qz);
		modelMatrix = modelMatrix.makeTranslation(t) * rotation * modelMatrix.makeScale(s);
	}
};

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	float time = glutGet(GLUT_ELAPSED_TIME);
	float t1 = -((float)time / 1000.0f);



	/*	Plot first Cube.	*/
	
	Cvec3 tr = { 0.0, 0.0, 0.0 };
	Cvec3 ro = { 0.0, 30.0, 0.0 };
	Cvec3 sc = { 1.0, 1.0, 1.0 };
	Entity Obj1(tr, ro, sc);
	Obj1.Transformation(1.0, t1, 1.0);

	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 35.0));

	Obj1.modelMatrix = inv(eyeMatrix) * Obj1.modelMatrix;

	GLfloat glmatrix[16];
	Obj1.modelMatrix.writeToColumnMajorMatrix(glmatrix);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, - 0.1, -100.0);

	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);
	
	Matrix4 normalMatrix;
	normalMatrix = transpose(inv(Obj1.modelMatrix));

	GLfloat glmatrixNormal[16];
	normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
	glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);

	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, normalPositionVBO);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 36); 



	/*	Plot child cube		*/
	
	tr = { 3.5, 3.5, 3.5 };
	ro = { 0.0, 30.0, 30.0 };
	sc = { 2.0, 2.0, 2.0 };
	Entity Obj2(tr, ro, sc);
	Obj2.parent = &Obj1;
	Obj2.Transformation(1.0, 1.0, t1);
	Obj2.modelMatrix = Obj2.parent->modelMatrix * Obj2.modelMatrix;

	Obj2.modelMatrix.writeToColumnMajorMatrix(glmatrix);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

	normalMatrix = transpose(inv(Obj2.modelMatrix));
	normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
	glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);

	glDrawArrays(GL_TRIANGLES, 0, 36);



	/*	Plot third Cube.	*/

	tr = { -5.0, -5.0, -1.5 };
	ro = { 0.0, 45.0, 60.0 };
	sc = { 3.0, 3.0, 3.0 };
	Entity Obj3(tr, ro, sc);
	Obj3.parent = &Obj2;
	Obj3.Transformation(1.0, 1.0, t1);
	Obj3.modelMatrix = Obj3.parent->modelMatrix * Obj3.modelMatrix;

	Obj3.modelMatrix.writeToColumnMajorMatrix(glmatrix);
	glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

	normalMatrix = transpose(inv(Obj3.modelMatrix));
	normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
	glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	
	/*Disabling Attributes.*/
	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(normalAttribute);
	
	glutSwapBuffers();
}

void init() {
	program = glCreateProgram();
	glClearDepth(0.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glCullFace(GL_BACK);
	glReadBuffer(GL_BACK);
	readAndCompileShader(program, "Vertex.glsl", "Fragment.glsl");
	

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	normalAttribute = glGetAttribLocation(program, "normal");
	modelViewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
	normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");



	glGenBuffers(1, &vertPostionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertPostionVBO);
	GLfloat cubeVerts[108] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,

		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,

		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,

		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,

		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeVerts, GL_STATIC_DRAW);


	glGenBuffers(1, &normalPositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalPositionVBO);
	GLfloat cubeNormals[108] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f,0.0f,
		
		0.0f, 0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f,0.0f,
		1.0f, 0.0f,0.0f,
		
		1.0f,0.0f,0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f, 0.0f,
		
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 1.0f,0.0f,
		
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f,0.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeNormals, GL_STATIC_DRAW);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CS - 6533 - 3D");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();
	glutMainLoop();
	return 0;
}