/*
	Auther		: Vatsal Biren Gopani
	NetID		: N17368916
	Subject		: Interactive Computer Graphics
	

*/

#define TINYOBJLOADER_IMPLEMENTATION

#include "glsupport.h"
#include "glut.h"
#include "iostream"
#include "matrix4.h"
#include "quat.h"
#include "math.h"
#include "vector"
#include "geometrymaker.h"
#include "tiny_obj_loader.h"

using namespace std;

GLuint program;
GLuint vertPostionVBO;
GLuint positionAttribute;
GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint normalPositionVBO;
GLuint normalAttribute;
GLuint normalMatrixUniformLocation;
GLuint VertexBO;
GLuint IndexBO;
GLuint color;
GLuint lightDirection[2];
GLuint lightColor[2];
GLuint specularLightColor[2];
GLuint texCoordAttribute;
GLuint diffuseTexture;
GLuint diffuseTextureUniformLocation;



/*
	This structure holds the Vertex data : vertices and normals position of the Object.
*/
struct VertexPN {
	Cvec3f p;
	Cvec3f n;
	Cvec2f t;

	VertexPN() {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : p(x,y,z), n(nx, ny, nz) {}

	VertexPN& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		t = v.tex;
		return *this;
	}
};



struct Light {
	Cvec3 lightColor;
	Cvec3 lightDirection;
	Cvec3 specularLightDirection;
};
Light lights[2];


struct Geometry{

	int numIndices;

	void Draw() {
		glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);

		glEnableVertexAttribArray(texCoordAttribute);
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, t));
		glEnableVertexAttribArray(texCoordAttribute);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBO);
		glDrawElements(GL_TRIANGLES, sizeof(VertexPN) * numIndices, GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(positionAttribute);
		glDisableVertexAttribArray(normalAttribute);
		glDisableVertexAttribArray(texCoordAttribute);
	}
};


class Entity {
public:

	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Matrix4 modelMatrix;
	Entity *parent;
	Geometry geometry;


	/*	This constructer sets the Object scale property to 1.0 by default.

		Arguments	: None (Default Constructer)

		Output		: Sets Scaling property of Object to 1.0.
	*/
	Entity(void) {
		s = {1.0, 1.0, 1.0};
		parent = NULL;
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
	void Transformation() {
		Quat Qx = Quat::makeXRotation(r[0]);
		Quat Qy = Quat::makeYRotation(r[1]);
		Quat Qz = Quat::makeZRotation(r[2]);
		Matrix4 rotation = quatToMatrix(Qx * Qy * Qz);
		
		if (parent != NULL)
		{
			modelMatrix = parent->modelMatrix * modelMatrix.makeTranslation(t) * rotation * modelMatrix.makeScale(s);
		}else{
			modelMatrix = modelMatrix.makeTranslation(t) * rotation * modelMatrix.makeScale(s);
		}
		

	}

	void Draw(Matrix4 &eyeInverse, Matrix4 &projectionMatrix, GLuint positionAttribute, GLuint normalAttribute, GLuint modelViewMatrixUniformLocation, GLuint normalMatrixUniformLocation) {
		Transformation();
	
		Matrix4 modelViewMatrix = eyeInverse * modelMatrix;

		GLfloat glmatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
		glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

		GLfloat glmatrixProjection[16];
		projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

		Matrix4 normalMatrix;
		normalMatrix = transpose(inv(modelMatrix));

		GLfloat glmatrixNormal[16];
		normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);

		geometry.Draw();

		glClearBufferData(GL_ARRAY_BUFFER, GL_FLOAT, NULL, GL_FLOAT, &VertexBO);
		glClearBufferData(GL_ARRAY_BUFFER, GL_UNSIGNED_SHORT, NULL, GL_UNSIGNED_SHORT, &IndexBO);
	}
};



/*	This function creates the 3D cube.
	
	Arguments	: This function takes the cube size as argument.

	Output		: The output of the function is 3D cube.

*/
void makeCube(float size) {
	int ibLen, vbLen;

	getCubeVbIbLen(vbLen, ibLen);

	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);

	makeCube(size, vtx.begin(), idx.begin());

	//glGenBuffers(1, &VertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexPN), vtx.data(), GL_STATIC_DRAW);

	//glGenBuffers(1, &IndexBO);
	glBindBuffer(GL_ARRAY_BUFFER, IndexBO);
	glBufferData(GL_ARRAY_BUFFER, idx.size() * sizeof(unsigned short), idx.data(), GL_STATIC_DRAW);
}



/*	This function creates the 3D Sphere.

Arguments	: This function takes the sphere radius as argument.

Output		: The output of the function is 3D Sphere.

*/
void makeSphere(float radius) {
	int ibLen, vbLen, slices = 10, stacks = 10;

	getSphereVbIbLen(slices, stacks, vbLen, ibLen);

	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);

	makeSphere(radius, slices, stacks, vtx.begin(), idx.begin());

	//glGenBuffers(1, &VertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexPN), vtx.data(), GL_STATIC_DRAW);

	//glGenBuffers(1, &IndexBO);
	glBindBuffer(GL_ARRAY_BUFFER, IndexBO);
	glBufferData(GL_ARRAY_BUFFER, idx.size() * sizeof(unsigned short), idx.data(), GL_STATIC_DRAW);
}



/*	This function creates the 3D Plane.

Arguments	: This function takes the Plane size as argument.

Output		: The output of the function is 3D Plane.

*/
void makePlane(float size) {
	int ibLen, vbLen;

	getPlaneVbIbLen(vbLen, ibLen);

	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);

	makePlane(size, vtx.begin(), idx.begin());

	
	glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexPN), vtx.data(), GL_STATIC_DRAW);

	
	glBindBuffer(GL_ARRAY_BUFFER, IndexBO);
	glBufferData(GL_ARRAY_BUFFER, idx.size() * sizeof(unsigned short), idx.data(), GL_STATIC_DRAW);
}

void loadObjFile(const std::string &fileName, std::vector<VertexPN> &outVertices, std::vector<unsigned short> &outIndices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	Geometry geometry;
	
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
	glEnableVertexAttribArray(positionAttribute);
	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
	glEnableVertexAttribArray(normalAttribute);
	glEnableVertexAttribArray(texCoordAttribute);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, t));
	glEnableVertexAttribArray(texCoordAttribute);
	
	
	
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
	
	


	if (ret){
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {

				unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
				unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
				unsigned int textOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
				VertexPN v;
				v.p[0] = attrib.vertices[vertexOffset];
				v.p[1] = attrib.vertices[vertexOffset + 1];
				v.p[2] = attrib.vertices[vertexOffset + 2];

				v.t[0] = attrib.texcoords[textOffset];
				v.t[1] = 1.0 - attrib.texcoords[textOffset + 1];

				v.n[0] = attrib.normals[normalOffset];
				v.n[1] = attrib.normals[normalOffset + 1];
				v.n[2] = attrib.normals[normalOffset + 2];
				
				outVertices.push_back(v);
				outIndices.push_back(outVertices.size() - 1);

/*
				glGenBuffers(1, &VertexBO);
				glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
				glGenBuffers(1, &IndexBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
		*/		

//				cout << "The vertice " << j << " has been loaded : " << v.p[0] << v.p[1] << v.p[2] << std::endl;
//				cout << "The normal " << j << " has been loaded : " << v.n[0] << v.n[1] << v.n[2] << std::endl;
//				cout << "The Texture" << j << "has been loaded : " << v.t[0] << v.t[1] << endl;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diffuseTexture);

				

//				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v.p[0]); //shapes[i].mesh.indices[j].vertex_index
//				glDrawElements(GL_TRIANGLES, sizeof(VertexPN), GL_UNSIGNED_SHORT, 0);

			}
		}

		
		
		cout << "Loaded";

		
	}
	else {
		std::cout << err << std::endl;
		assert(false);
	}

	

}


void make3DObject() {
	std::vector<VertexPN> vtx;
	std::vector<unsigned short> idx;

	
	// Monk_Giveaway/Monk_Giveaway_Fixed.obj

	loadObjFile("Monk_Giveaway/Monk_Giveaway_Fixed.obj", vtx, idx);

	//glGenBuffers(1, &VertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexPN), vtx.data(), GL_STATIC_DRAW);

	//glGenBuffers(1, &IndexBO);
	glBindBuffer(GL_ARRAY_BUFFER, IndexBO);
	glBufferData(GL_ARRAY_BUFFER, idx.size() * sizeof(unsigned short), idx.data(), GL_STATIC_DRAW);

	glUniform1i(diffuseTextureUniformLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);


	glEnableVertexAttribArray(texCoordAttribute);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, t));

}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	float time = glutGet(GLUT_ELAPSED_TIME);
	float t1 = ((float)time / 1000.0f);


	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 35.0));
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);
	Matrix4 eyeInverse = inv(eyeMatrix);

/*
	Cvec3 tr = { 0.0, -8.0, 0.0 };
	Cvec3 ro = { 0.0, 45.0 * t1, 0.0 };
	Cvec3 sc = { 1.0, 1.0, 1.0 };
	Entity ObjBase(tr, ro, sc);
	ObjBase.parent = NULL;
	ObjBase.geometry.numIndices = 4;
	makePlane(15);
	ObjBase.Draw(eyeInverse, projectionMatrix, positionAttribute, normalAttribute, modelViewMatrixUniformLocation, normalMatrixUniformLocation);
	*/

	Cvec3 tr = { 0.0, -5.0, 0.0 };
	Cvec3 ro = { 0.0, 10.0 * t1, 0.0 };
	Cvec3 sc = { 1.0, 1.0, 1.0 };
	Entity Obj1(tr, ro, sc);
	Obj1.parent = NULL;
	Obj1.geometry.numIndices = 30152; 
	make3DObject();
	Obj1.Draw(eyeInverse, projectionMatrix, positionAttribute, normalAttribute, modelViewMatrixUniformLocation, normalMatrixUniformLocation);
	

	/*Disabling Attributes.	*/	
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(normalAttribute);
	glDisableVertexAttribArray(texCoordAttribute);
	
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
	color = glGetUniformLocation(program, "uColor");

	glUniform3f(color, 1.0, 1.2, 1.0);

	lightColor[0] = glGetUniformLocation(program, "lights[0].lightColor");
	lightDirection[0] = glGetUniformLocation(program, "lights[0].lightPosition");
	specularLightColor[0] = glGetUniformLocation(program, "lights[0].specularLightColor");
	glUniform3f(lightDirection[0], -1.0, 1.0, 1.0);
	glUniform3f(lightColor[0], 1.0, 0.0, 1.0);
	glUniform3f(specularLightColor[0], 1.0, 1.0, 1.0);
	
	lightColor[1] = glGetUniformLocation(program, "lights[1].lightColor");
	lightDirection[1] = glGetUniformLocation(program, "lights[1].lightPosition");
	specularLightColor[1] = glGetUniformLocation(program, "lights[1].specularLightColor");
	glUniform3f(lightDirection[1], 1.0, 1.0, 1.0);
	glUniform3f(lightColor[1], 1.0, 1.0, 0.0);
	glUniform3f(specularLightColor[1], 1.0, 1.0, 1.0);

	diffuseTexture = loadGLTexture("Monk_Giveaway/Monk_D.tga");
	diffuseTextureUniformLocation = glGetUniformLocation(program, "diffuseTexture");
	glUniform1i(diffuseTextureUniformLocation, 0);

	

	glGenBuffers(1, &VertexBO);
	glGenBuffers(1, &IndexBO);
	
	
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