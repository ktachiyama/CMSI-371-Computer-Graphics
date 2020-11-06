/***
 Assignment-3: Geometric Modeling of a Scene

 Name: Krysten Tachiyama

 Project Summary:
	 For this project, I replicated the real (anime) world in 3D space using 
 hierarchical modeling. The baseis for all of the objects created is a unit cube. From this 
 cube, similar objects were made that in turn created larger objects (e.g. cubes were
 turned into planks, which were combined to make a flooring). Colors were also
 defined and assigned to each plane of a cube object.

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

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <array>
using namespace std;

#define M_PI 3.14159265358979323846
// If a float is < EPSILON or > -EPILSON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 3.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

struct Colors {
	array<GLfloat, 12> lightBrown = { 0.7, 0.5, 0.0,
									0.7, 0.5, 0.0,
									0.7, 0.5, 0.0,
									0.7, 0.5, 0.0 };

	array<GLfloat, 12> brown = { 0.6, 0.4, 0.1,
								0.6, 0.4, 0.1,
								0.6, 0.4, 0.1,
								0.6, 0.4, 0.1 };

	array<GLfloat, 12> teal = { 0.2, 0.4, 0.5,
								0.2, 0.4, 0.5, 
								0.2, 0.4, 0.5, 
								0.2, 0.4, 0.5 };

	array<GLfloat, 12> darkTeal = { 0.2, 0.3, 0.4,
									0.2, 0.3, 0.4,
									0.2, 0.3, 0.4,
									0.2, 0.3, 0.4 };

	array<GLfloat, 12> cream = { 0.9, 0.9, 0.9,
								0.9, 0.9, 0.9,
								0.9, 0.9, 0.9,
								0.9, 0.9, 0.9 };

	array<GLfloat, 12> maroon = { 0.7, 0.1, 0.2,
								0.7, 0.1, 0.2, 
								0.7, 0.1, 0.2, 
								0.7, 0.1, 0.2 };

	array<GLfloat, 12> orangishMaroon = { 0.8, 0.3, 0.2,
										0.8, 0.3, 0.2,
										0.8, 0.3, 0.2, 
										0.8, 0.3, 0.2};

	array<GLfloat, 12> darkMaroon = { 0.6, 0, 0.1,
									0.6, 0, 0.1, 
									0.6, 0, 0.1, 
									0.6, 0, 0.1 };

	array<GLfloat, 12> white = { 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 
								1.0, 1.0, 1.0, 
								1.0, 1.0, 1.0 };
};


/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

 // Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
	vector<GLfloat> vertices = {
		+0.5,   +0.5,   +0.0,
		-0.5,   +0.5,   +0.0,
		-0.5,   -0.5,   +0.0,
		+0.5,   -0.5,   +0.0
	};
	return vertices;
}

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
vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
	vector<GLfloat> result;
	for (int i = 0; i < cartesian_coords.size(); i++) {
		result.push_back(cartesian_coords[i]);
		if ((i + 1) % 3 == 0) {
			result.push_back(1.0);
		}
	}
	return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
	vector<GLfloat> result;
	for (int i = 0; i < homogeneous_coords.size(); i++) {
		if ((i + 1) % 4 == 0) {
			continue;
		}
		else {
			result.push_back(homogeneous_coords[i]);
		}
	}
	return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix(float dx, float dy, float dz) {
	// TODO: Define translation matrix
	vector<GLfloat> translate_mat = { 1, 0, 0, dx,
									  0, 1, 0, dy,
									  0, 0, 1, dz,
									  0, 0, 0, 1 };
	return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix(float sx, float sy, float sz) {
	// TODO: Define scaling matrix
	vector<GLfloat> scale_mat = { sx, 0, 0, 0,
								  0, sy, 0, 0,
								  0, 0, sz, 0,
								  0, 0, 0, 1 };
	return scale_mat;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x(float theta) {
	// TODO: Define rotation matrix about the x-axis matrix
	theta = deg2rad(theta);
	vector<GLfloat> rotate_mat_x = { 1, 0, 0, 0,
									 0, cos(theta), -sin(theta), 0,
									 0, sin(theta), cos(theta), 0,
									 0, 0, 0, 1 };
	return rotate_mat_x;
}

// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y(float theta) {
	// TODO: Define rotation matrix about the y-axis matrix
	theta = deg2rad(theta);
	vector<GLfloat> rotate_mat_y = { cos(theta), 0, sin(theta), 0,
									 0, 1, 0, 0,
									 -sin(theta), 0, cos(theta), 0,
									 0, 0, 0, 1 };
	return rotate_mat_y;
}

// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z(float theta) {
	// TODO: Define rotation matrix about the z-axis matrix
	theta = deg2rad(theta);
	vector<GLfloat> rotate_mat_z = { cos(theta), -sin(theta), 0, 0,
									 sin(theta), cos(theta), 0, 0,
									 0, 0, 1, 0,
									 0, 0, 0, 1 };

	return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
	vector<GLfloat> result;
	for (int i = 0; i < B.size(); i += 4) {
		for (int j = 0; j < A.size(); j += 4) {
			GLfloat dot_product = 0.0;
			for (int k = 0; k < 4; k++) {
				GLfloat value = A[j + k] * B[i + k];
				if (value < EPSILON && value > -1.0*EPSILON) {
					value = 0.0;
				}
				dot_product += value;
			}
			result.push_back(dot_product);
		}
	}
	return result;
}

void addColorToScene(array<GLfloat, 12> color) {
	for (GLfloat c : color) {
		COLOR.push_back(c);
	}
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube(float xLength, float yLength, float zLength) {
	// TODO: Creates a unit cube by transforming a set of planes

	vector<GLfloat> result;
	vector<vector<GLfloat>> sides;

	vector<GLfloat> frontplane = to_cartesian_coord(mat_mult(translation_matrix(0.0f, 0.0f, 0.5 * zLength), mat_mult(scaling_matrix(xLength, yLength, 0.0f), to_homogeneous_coord(init_plane()))));
	vector<GLfloat> backplane = to_cartesian_coord(mat_mult(translation_matrix(0.0f, 0.0f, -0.5 * zLength), mat_mult(rotation_matrix_y(180.0f), mat_mult(scaling_matrix(xLength, yLength, 0.0f), to_homogeneous_coord(init_plane())))));
	vector<GLfloat> rightside = to_cartesian_coord(mat_mult(translation_matrix(0.5 * xLength, 0.0f, 0.0f), mat_mult(rotation_matrix_y(90.0f), mat_mult(scaling_matrix(zLength, yLength, 0.0f), to_homogeneous_coord(init_plane())))));
	vector<GLfloat> leftside = to_cartesian_coord(mat_mult(translation_matrix(-0.5 * xLength, 0.0f, 0.0f), mat_mult(rotation_matrix_y(90.0f), mat_mult(scaling_matrix(zLength, yLength, 0.0f), to_homogeneous_coord(init_plane())))));
	vector<GLfloat> top = to_cartesian_coord(mat_mult(translation_matrix(0.0f, 0.5 * yLength, 0.0f), mat_mult(rotation_matrix_x(-90.0f), mat_mult(scaling_matrix(xLength, zLength, 0.0f), to_homogeneous_coord(init_plane())))));
	vector<GLfloat> bottom = to_cartesian_coord(mat_mult(translation_matrix(0.0f, -0.5 * yLength, 0.0f), mat_mult(rotation_matrix_x(90.0f), mat_mult(scaling_matrix(xLength, zLength, 0.0f), to_homogeneous_coord(init_plane())))));

	sides.push_back(frontplane);
	sides.push_back(backplane);
	sides.push_back(rightside);
	sides.push_back(leftside);
	sides.push_back(top);
	sides.push_back(bottom);

	for (vector<GLfloat> v : sides) {
		result.insert(result.end(), v.begin(), v.end());
	}

	/*cout << "---------------------------" << endl;
	int incrament = 0;
	for (GLfloat p: result) {
		if (incrament % 3 == 0) {
			cout << endl;
		}
		if (incrament > 11) {
			cout << endl << "next plane" << endl;
			incrament = 0;
		}
		cout << p << ",  ";
		incrament++;
	}
	cout << endl << "---------------------------" << endl;*/

	return result;
}

vector<GLfloat> buildPlank(float xPos, float yPos, float zPos) {
	float plankWidth = 0.9f; //x distance
	float plankHeight = 0.5f; //y distance
	float plankLength = 16.0f; //z distance

	Colors c = Colors();
	addColorToScene(c.lightBrown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.lightBrown);
	addColorToScene(c.brown);

	return to_cartesian_coord(mat_mult(translation_matrix(xPos, yPos, zPos), 
		to_homogeneous_coord(build_cube(plankWidth, plankHeight, plankLength))));
}

vector<GLfloat> buildSideWall(float xPos, float yPos, float zPos) {
	vector<GLfloat> sideWall;

	float frontBarWidth = 1.0f; //x distance
	float frontBarHeight = 10.5f; //y distance
	float frontBarLength = 0.5f; //z distance

	Colors c = Colors();
	addColorToScene(c.teal);
	addColorToScene(c.teal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);

	 vector<GLfloat> frontBar = to_cartesian_coord(mat_mult(translation_matrix(xPos, yPos, zPos + 7.75),
		to_homogeneous_coord(build_cube(frontBarWidth, frontBarHeight, frontBarLength))));

	float bottomWallWidth = 1.0f; //x distance
	float bottomWallHeight = 4.5f; //y distance
	float bottomWallLength = 15.5f; //z distance

	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);

	vector<GLfloat> bottomWall = to_cartesian_coord(mat_mult(translation_matrix(xPos, yPos - 3.0, zPos - 0.25),
		to_homogeneous_coord(build_cube(bottomWallWidth, bottomWallHeight, bottomWallLength))));

	float topWallWidth = 1.0f; //x distance
	float topWallHeight = 6.0f; //y distance
	float topWallLength = 15.5f; //z distance

	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);

	vector<GLfloat> topWall = to_cartesian_coord(mat_mult(translation_matrix(xPos, yPos + 2.25, zPos - 0.25),
		to_homogeneous_coord(build_cube(topWallWidth, topWallHeight, topWallLength))));

	sideWall.insert(sideWall.end(), frontBar.begin(), frontBar.end());
	sideWall.insert(sideWall.end(), bottomWall.begin(), bottomWall.end());
	sideWall.insert(sideWall.end(), topWall.begin(), topWall.end());

	return sideWall;
}

vector<GLfloat> buildFlags(float xPos, float yPos, float zPos) {
	float flagWidth = 3.0f; //x distance
	float flagHeight = 4.0f; //y distance
	float flagLength = 0.1f; //z distance

	Colors c = Colors();
	addColorToScene(c.white);
	addColorToScene(c.white);
	addColorToScene(c.white);
	addColorToScene(c.white);
	addColorToScene(c.white);
	addColorToScene(c.white);

	return to_cartesian_coord(mat_mult(translation_matrix(xPos, yPos, zPos),
		to_homogeneous_coord(build_cube(flagWidth, flagHeight, flagLength))));
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by setting       *
 *  the projection/viewing matrices               *
 *                                                *
 *************************************************/

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

	// TODO: Setup your camera here

	// Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
	gluPerspective(50.0, 1.0, 2.0, 60.0);
	// Position camera at (0, 2, 5), attention at (0, 0, 0), up at (0, 1, 0)
	gluLookAt(0, 7, 45, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void rotateCamera(GLfloat theta) {
	glRotatef(theta, 0, 1, 0);
}

// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
	// TODO: Build your scene here
	vector<GLfloat> scene;
	Colors c = Colors();

	//////////////////// floor //////////////////////
	vector<vector<GLfloat>> floor;
	float plank_xPos = -6.5f;
	float plank_yPos = -8.25f;
	float plank_zPos = 0.0f;
	int numPlanks = 14;
	float plankWidth = 1.0f;
	for (int i = 0; i < numPlanks; i++) {
		floor.push_back(buildPlank(plank_xPos, plank_yPos, plank_zPos));
		plank_xPos += plankWidth;
	}

	for (vector<GLfloat> plank : floor) {
		for (GLfloat point : plank) {
			scene.push_back(point);
		}
	}
	/////////////////////////////////////////////////
	
	////////////////// side walls ////////////////////
	float rwall_xPos = 7.5f;
	float rwall_yPos = -3.25f;
	float rwall_zPos = 0.0f;

	float lwall_xPos = -7.5f;
	float lwall_yPos = -3.25f;
	float lwall_zPos = 0.0f;

	for (GLfloat f : buildSideWall(rwall_xPos, rwall_yPos, rwall_zPos)) {
		scene.push_back(f);
	}
	for (GLfloat f : buildSideWall(lwall_xPos, lwall_yPos, lwall_zPos)) {
		scene.push_back(f);
	}
	/////////////////////////////////////////////

	////////////////// back wall ////////////////////
	float bWall_xPos = 0.0;
	float bWall_zPos = -7.5f;
	float bWall_width = 14.0f;
	float bWall_length = 1.0f;

	float bTopwall_height = 6.0f;
	float bTopwall_yPos = -1.0f;

	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);
	addColorToScene(c.cream);

	vector<GLfloat> bTopWall = to_cartesian_coord(mat_mult(translation_matrix(bWall_xPos, bTopwall_yPos, bWall_zPos),
		to_homogeneous_coord(build_cube(bWall_width, bTopwall_height, bWall_length))));

	for (GLfloat f : bTopWall) {
		scene.push_back(f);
	}

	float bBotwall_height = 4.5f;
	float bBotwall_yPos = -6.25f;

	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);
	addColorToScene(c.brown);

	vector<GLfloat> bBotWall = to_cartesian_coord(mat_mult(translation_matrix(bWall_xPos, bBotwall_yPos, bWall_zPos),
		to_homogeneous_coord(build_cube(bWall_width, bBotwall_height, bWall_length))));

	for (GLfloat f : bBotWall) {
		scene.push_back(f);
	}

	////////////////////////////////////////////////

	////////////////// counter ////////////////////
	float counter_xPos = 0.0;
	float backCounter_yPos = -5.5f;
	float backCounter_zPos = 2.0f;
	float backCounterWidth = 14.0f; //x distance
	float backCounterHeight = 5.0f; //y distance
	float backCounterLength = 2.0f; //z distance


	//// back of counter /////
	addColorToScene(c.orangishMaroon);
	addColorToScene(c.darkMaroon);
	addColorToScene(c.maroon);
	addColorToScene(c.maroon);
	addColorToScene(c.darkMaroon);
	addColorToScene(c.darkMaroon);

	vector<GLfloat> backCounter = to_cartesian_coord(mat_mult(translation_matrix(counter_xPos, backCounter_yPos, backCounter_zPos),
		to_homogeneous_coord(build_cube(backCounterWidth, backCounterHeight, backCounterLength))));

	for (GLfloat f : backCounter) {
		scene.push_back(f);
	}

	///// counter top /////

	float topCounter_yPos = -4.75f;
	float topCounter_zPos = 4.0f;
	float topCounterWidth = 14.0f; //x distance
	float topCounterHeight = 0.5f; //y distance
	float topCounterLength = 2.0f; //z distance

	addColorToScene(c.maroon);
	addColorToScene(c.darkMaroon);
	addColorToScene(c.maroon);
	addColorToScene(c.maroon);
	addColorToScene(c.darkMaroon);
	addColorToScene(c.darkMaroon);

	vector<GLfloat> topCounter = to_cartesian_coord(mat_mult(translation_matrix(counter_xPos, topCounter_yPos, topCounter_zPos),
		to_homogeneous_coord(build_cube(topCounterWidth, topCounterHeight, topCounterLength))));

	for (GLfloat f : topCounter) {
		scene.push_back(f);
	}
	//////////////////////////////////////////////

	////////////////// ceiling ////////////////////
	float roofWidth = 18.0f;
	float roofHeight = 1.0f;
	float roofLength = 18.0f;

	float roof_xPos = 0.0f;
	float roof_yPos = 2.5f;
	float roof_zPos = 0.0f;

	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.darkTeal);
	addColorToScene(c.teal);
	addColorToScene(c.darkTeal);

	vector<GLfloat> ceiling = to_cartesian_coord(mat_mult(translation_matrix(roof_xPos, roof_yPos, roof_zPos),
		to_homogeneous_coord(build_cube(roofWidth, roofHeight, roofLength))));

	for (GLfloat f : ceiling) {
		scene.push_back(f);
	}
	//////////////////////////////////////////

	/////////////////  flags  ///////////////
	vector<vector<GLfloat>> flags;
	float flag_xPos = -6.4f;
	float flag_yPos = 0.0f;
	float flag_zPos = 8.0f;
	int numFlags = 5;
	float flagWidth = 3.2f;
	for (int i = 0; i < numFlags; i++) {
		flags.push_back(buildFlags(flag_xPos, flag_yPos, flag_zPos));
		flag_xPos += flagWidth;
	}

	for (vector<GLfloat> flag : flags) {
		for (GLfloat point : flag) {
			scene.push_back(point);
		}
	}
	////////////////////////////////////////

	return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
	vector<GLfloat> colors;
	for (int i = 0; i < scene.size(); i++) {
		colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}
	return colors;
}

/*While rotateScene() works, I didn't use it to rotate the scene
because regenerating all of the points made everything really slow
and choppy. Instead, I rotated the camera around the scene itself.*/
void rotateScene(vector<GLfloat> &points, float degree) {
	vector<GLfloat> rotationMatrix = rotation_matrix_y(degree);
	vector<GLfloat> rotatedPoints;

	//make homogeneous
	points = to_homogeneous_coord(points);

	//rotate each point by multiplying with rotation matrix
	vector<GLfloat> temp;
	for (int i = 0; i < points.size(); i++) {
		temp.push_back(points[i]);
		if ((i + 1) % 16 == 0) {
			temp = mat_mult(rotationMatrix, temp);
			rotatedPoints.insert(rotatedPoints.end(), temp.begin(), temp.end());
			temp.clear();
		}
	}

	//make cartesian
	points = to_cartesian_coord(rotatedPoints);
}

void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Rotate the scene using the scene vector
	vector<GLfloat> scene = init_scene();
	//rotateScene(scene, THETA);
	rotateCamera(THETA);

	GLfloat* scene_vertices = vector2array(scene);
	GLfloat* color_vertices = vector2array(COLOR);

	scene.clear();
	COLOR.clear();

	 //Pass the scene vertex pointer
	glVertexPointer(3,                // 3 components (x, y, z)
		GL_FLOAT,         // Vertex type is GL_FLOAT
		0,                // Start position in referenced memory
		scene_vertices);  // Pointer to memory location to read from

	// Pass the color vertex pointer
	glColorPointer(3,                   // 3 components (r, g, b)
		GL_FLOAT,            // Vertex type is GL_FLOAT
		0,                   // Start position in referenced memory
		color_vertices);     // Pointer to memory location to read from

	// Draw quad point planes: each 4 vertices
	glDrawArrays(GL_QUADS, 0, SCENE.size() / 3.0);

	glFlush();			//Finish rendering
	glutSwapBuffers();

	delete scene_vertices;
	delete color_vertices;
}

void idle_func() {
	//THETA = THETA + 0.2f;
	display_func();
}

int main(int argc, char **argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	// Create a window with rendering context and everything else we need
	glutCreateWindow("Assignment 3");

	setup();
	init_camera();

	// Setting global variables SCENE and COLOR with actual values
	SCENE = init_scene();
	//COLOR = init_color(SCENE);

	// Set up our display function
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	// Render our world
	glutMainLoop();

	// Remember to call "delete" on your dynmically allocated arrays
	// such that you don't suffer from memory leaks. e.g.
	// delete arr;

	return 0;
}