/***
 Assignment-2: Rotating a Cube in 3-Dimensional Space

 Name: Tachiyama, Krysten

 Collaborators: none

 Project Summary: 
	This project declares a vector of 72 entries, or 21 points, to represent a cube. The
	cube is rotated a specified degree around either the x, y, or z axis. This is achieved by turning the 
	1 dimensional vector into a 2 dimensional vector (or matrix) in which each row has 3 entries to 
	represent a point (e.g. [1, 1, 1]). These points are turned into homogenious coordinates and then 
	multiplied by a rotation matrix, allowing the entire cube to rotate.
 ***/

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef _WIN32
#include <GL\glew.h>
#include <GL\freeglut.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#pragma GCC diagnostic pop

#include<iostream>
#include <math.h>
#include <vector>
#include <array>

#define M_PI 3.14159265358979323846

using namespace std;

enum Direction  { x, y, z };
float theta = 0.0;
const int NUM_OF_ELEMENTS = 72;

float deg2rad(float d);
GLfloat* vector2array(vector<GLfloat> vec);
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords);
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords);
vector<GLfloat> rotation_matrix_x(float theta);
vector<GLfloat> rotation_matrix_y(float theta);
vector<GLfloat> rotation_matrix_z(float theta);
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B);
void rotate_points(vector<GLfloat> &points, float degree, Direction direction);
vector<GLfloat> to_1Dvector(vector<vector<GLfloat>> vec2D);
array<GLfloat, NUM_OF_ELEMENTS> to_array(vector<vector<GLfloat>>);

// Converts degrees to radians for rotation
float deg2rad(float d) {
	return (d*M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
	GLfloat* arr = new GLfloat[vec.size()];
	for (int i = 0; i < vec.size(); i++) {
		arr[i] = vec[i];
	}
	return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
	vector<GLfloat> result(cartesian_coords);

	// TODO: Append the 1 in the 4th dimension to generate homoegenous coordinates

	result.push_back(1);

	return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
	vector<GLfloat> result(homogenous_coords);

	//// TODO: Remove the 1 in the 4th dimension to generate Cartesian coordinates
	result.pop_back();

	return result;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x(float theta) {
	
	// TODO: Define the rotation matrix about the x-axis
	vector<GLfloat> rotate_mat_x = { 1, 0, 0, 0,
									 0, cos(theta), -sin(theta), 0,
									 0, sin(theta), cos(theta), 0,
									 0, 0, 0, 1 };

	return rotate_mat_x;
}

// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y(float theta) {
	// TODO: Define the rotation matrix about the y-axis
	vector<GLfloat> rotate_mat_y = { cos(theta), 0, sin(theta), 0,
									 0, 1, 0, 0,
									 -sin(theta), 0, cos(theta), 0,
									 0, 0, 0, 1 };

	return rotate_mat_y;
}

// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z(float theta) {
	// TODO: Define the rotation matrix about the z-axis
	vector<GLfloat> rotate_mat_z = { cos(theta), -sin(theta), 0, 0,
									 sin(theta), cos(theta), 0, 0,
									 0, 0, 1, 0,
									 0, 0, 0, 1 };
	return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
	vector<GLfloat> result;

	// TODO: Compute matrix multiplication of A B

	int i = 0;
	int sizeOfA = A.size();

	while (i < sizeOfA) {
		GLfloat input = 0;
		for (GLfloat f : B) {
			input += (f * A[i]);
			i++;
		}
		result.push_back(input);
	}

	return result;
}

//takes in a 1D vector and returns a 2D vector of GLfloats
vector<vector<GLfloat>> to_2Dvector(const vector<GLfloat> &vec) {
	vector<vector<GLfloat>> results;
	int i = 0;
	int numOfElements = vec.size();

	while (i < numOfElements) {
		vector<GLfloat> temp;
		for (int j = 0; j < 3 && i < numOfElements; j++) {
			temp.push_back(vec[i]);
			i++;
		}
		results.push_back(temp);
	}
	return results;
}

//takes in a 2D vector and returns a flattened 1D vector
vector<GLfloat> to_1Dvector(vector<vector<GLfloat>> vec2D) {
	vector<GLfloat> result;
	for (vector<GLfloat> v : vec2D) {
		for (GLfloat f : v) {
			result.push_back(f);
		}
	}

	return result;
}

//rotates the points a certain degree around either and x, y, or z direction
void rotate_points(vector<GLfloat> &points, float degree, Direction direction) {
	float radian = deg2rad(degree);

	//turn points from a 1D vector to 2D
	vector<vector<GLfloat>> pointsVec = to_2Dvector(points);

	//make homogenious
	for (int i = 0; i < pointsVec.size(); i++) {
		pointsVec[i] = to_homogenous_coord(pointsVec[i]);
	}

	vector<GLfloat> rotationMatrix;

	switch (direction) {
	case x:
		rotationMatrix = rotation_matrix_x(radian);
		break;
	case y:
		rotationMatrix = rotation_matrix_y(radian);
		break;
	case z:
		rotationMatrix = rotation_matrix_z(radian);
		break;
	default: cerr << "something went wrong";
	}

	//rotate each point by multiplying with rotation matrix
	for (int i = 0; i < pointsVec.size(); i++) {
		pointsVec[i] = mat_mult(rotationMatrix, pointsVec[i]);
	}

	//make cartesian
	for (int i = 0; i < pointsVec.size(); i++) {
		pointsVec[i] = to_cartesian_coord(pointsVec[i]);
	}

	points = to_1Dvector(pointsVec);
}

void setup() {
	// Enable the vertex array functionality
	glEnableClientState(GL_VERTEX_ARRAY);
	// Enable the color array functionality (so we can specify a color for each vertex)
	glEnableClientState(GL_COLOR_ARRAY);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Set up some default base color
	glColor3f(0.5, 0.5, 0.5);
	// Set up white background
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
	// Camera parameters
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
	gluPerspective(50.0, 1.0, 2.0, 10.0);
	// Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
	gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// World model parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vector<GLfloat> points = {
		// Front plane
		+1.0,   +1.0,   +1.0,
		-1.0,   +1.0,   +1.0,
		-1.0,   -1.0,   +1.0,
		+1.0,   -1.0,   +1.0,
		// Back plane
		+1.0,   +1.0,   -1.0,
		-1.0,   +1.0,   -1.0,
		-1.0,   -1.0,   -1.0,
		+1.0,   -1.0,   -1.0,
		// Right
		+1.0,   +1.0,   -1.0,
		+1.0,   +1.0,   +1.0,
		+1.0,   -1.0,   +1.0,
		+1.0,   -1.0,   -1.0,
		// Left
		-1.0,   +1.0,   -1.0,
		-1.0,   +1.0,   +1.0,
		-1.0,   -1.0,   +1.0,
		-1.0,   -1.0,   -1.0,
		// Top
		+1.0,   +1.0,   +1.0,
		-1.0,   +1.0,   +1.0,
		-1.0,   +1.0,   -1.0,
		+1.0,   +1.0,   -1.0,
		// Bottom
		+1.0,   -1.0,   +1.0,
		-1.0,   -1.0,   +1.0,
		-1.0,   -1.0,   -1.0,
		+1.0,   -1.0,   -1.0,
	};

	GLfloat colors[] = {
		// Front plane
		1.0,    0.5,    0.6,
		1.0,    0.5,    0.6,
		1.0,    0.5,    0.6,
		1.0,    0.5,    0.6,
		// Back plane
		1.0,    0.8,    0.4,
		1.0,    0.8,    0.4,
		1.0,    0.8,    0.4,
		1.0,    0.8,    0.4,
		// Right
		0.0,    0.9,    0.8,
		0.0,    0.9,    0.8,
		0.0,    0.9,    0.8,
		0.0,    0.9,    0.8,
		// Left
		1.0,    1.0,    0.0,
		1.0,    1.0,    0.0,
		1.0,    1.0,    0.0,
		1.0,    1.0,    0.0,
		// Top
		0.7,    1.0,    0.8,
		0.7,    1.0,    0.8,
		0.7,    1.0,    0.8,
		0.7,    1.0,    0.8,
		// Bottom
		0.0,    1.0,    1.0,
		0.0,    1.0,    1.0,
		0.0,    1.0,    1.0,
		0.0,    1.0,    1.0,
	};
	
	/////////// TODO: Apply rotation(s) to the set of points  /////////////

	rotate_points(points, theta, x);
	rotate_points(points, theta, y);
	theta += 0.2;

	///////////////////////////////////////////////////////////////////////

	GLfloat* vertices = vector2array(points);

	glVertexPointer(3,  // 3 components (x, y, z)
		GL_FLOAT,		// Vertex type is GL_FLOAT
		0,				// Start position in referenced memory
		vertices);		// Pointer to memory location to read from

	//pass the color pointer
	glColorPointer(3,   // 3 components (r, g, b)
		GL_FLOAT,		// Vertex type is GL_FLOAT
		0,				// Start position in referenced memory
		colors);		// Pointer to memory location to read from

	// Draw quad point planes: each 4 vertices
	glDrawArrays(GL_QUADS, 0, 4 * 6);

	glFlush();			//Finish rendering
	glutSwapBuffers();

	delete vertices;
}

void idle_func() {
	theta = theta + 0.3;
	display_func();
}

int main(int argc, char **argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	// Create a window with rendering context and everything else we need
	glutCreateWindow("Assignment 2");

	setup();
	init_camera();

	// Set up our display function
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	// Render our world
	glutMainLoop();
	return 0;
}
