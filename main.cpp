#include <GL/glut.h>
#include <math.h>
#include <SOIL2.h>
#include <stdio.h>

constexpr float PI = 3.14159265358979323846;

//Define textures
GLuint tex;

// Texture image rows and columns for checker board pattern
#define IMAGE_ROWS 64
#define IMAGE_COLS 64

// Texture - checker board pattern image array
GLubyte imageData[IMAGE_ROWS][IMAGE_COLS][3];

//To on/off grids and axes
int gridOn = 0;
int axesOn = 0;

//variables to move the camera
GLfloat camX = 0.0; GLfloat camY = 0.0; GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

////variables to  move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

// Load the imageData array with checkerboad pattern
void loadTextureImageData() {
	int value;
	for (int row = 0; row < IMAGE_ROWS; row++) {
		for (int col = 0; col < IMAGE_COLS; col++) {
			// Each cell is 8x8, value is 0 or 255 (black or white)
			value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
			imageData[row][col][0] = (GLubyte)value;
			imageData[row][col][1] = (GLubyte)value;
			imageData[row][col][2] = (GLubyte)value;
		}
	}
}

//Load and bind texures
void loadTextures() {

	loadTextureImageData();   // Load pattern into image data array
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
		GL_UNSIGNED_BYTE, imageData);  // Create texture from image data

	// Texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void drawGrid() {
	GLfloat step = 1.0f;
	GLint line;

	glBegin(GL_LINES);
	for (line = -20; line <= 20; line += step) {
		glVertex3f(line, -0.4, 20);
		glVertex3f(line, -0.4, -20);

		glVertex3f(20, -0.4, line);
		glVertex3f(-20, -0.4, line);
	}
	glEnd();
}

void drawAxes() {

	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-200, 0, 0);
	glVertex3f(200, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -200, 0);
	glVertex3f(0, 200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -200);
	glVertex3f(0, 0, 200);

	glEnd();
}

void init(void) {
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	//Enabling texture
	glEnable(GL_TEXTURE_2D);

	loadTextures();
}

void drawWheelSpokes() {
	double theta = 45;
	int n = 6;

	GLUquadric* quad = gluNewQuadric();

	for (int i = 0; i <= 360; i += 45) {
		glVertex2f(0, 0);
		glPushMatrix();
		glRotatef(i, 1, 0, 0);
		gluCylinder(quad, 0.05, 0.05, 0.95, 100, 100);
		glPopMatrix();
	}

	gluDeleteQuadric(quad);


}

void drawTorus() {
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glutSolidTorus(0.15, 1, 32, 32);
	glPopMatrix();

	GLUquadric* quad = gluNewQuadric();
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -0.08);
	gluCylinder(quad, 0.2, 0.2, 0.2, 100, 100);
	glPopMatrix();
	gluDeleteQuadric(quad);
}

void drawWheel() {

	glColor3f(0, 0, 0);
	drawTorus();

	glColor3f(0.9, 0.9, 0.9);
	drawWheelSpokes();
}

void drawTube() {
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, 0.05, 0.05, 2, 100, 100);
	gluDeleteQuadric(quad);
}

void drawTubes() {

	glColor3f(0.9, 0.4, 0.5);
	drawTube();

	glPushMatrix();
	glRotatef(65, 1, 0, 0);
	glScalef(1, 1, 1.75);
	drawTube();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-80, 1, 0, 0);
	glScalef(1, 1, 1.6);
	glTranslatef(0, -2, -1.75);
	drawTube();
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 1, 2);
	glTranslatef(0, -3.25, 0.75);
	drawTube();
	glPopMatrix();
}

void drawSeatTray() {
	glColor3f(0.8, 0.7, 0.6);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	drawTube();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.2);
	glTranslatef(0, -4, -1);
	drawTube();
	glPopMatrix();

	double scale = 0.75;
	double gap = 0.25;
	double height = 2.70;

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(scale, scale, scale);
	glTranslatef(gap, 0, height);
	drawTube();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(scale, scale, scale);
	glTranslatef(-gap, 0, height);
	drawTube();
	glPopMatrix();
}

void drawHandleBar() {

	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glScalef(1.5, 1.5, 1);
	drawTube();
	glPopMatrix();

	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glScalef(3, 3, 0.5);
	drawTube();
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(0, 0, 2);
	glScalef(3, 3, 0.5);
	drawTube();
	glPopMatrix();

}

void drawSeat() {
	GLUquadric* quad = gluNewQuadric();
	glColor3f(0.1, 0.1, 0.5);
	gluSphere(quad, 1, 100, 100);
}

void drawBicycle() {
	glPushMatrix();
	glTranslatef(0, -3, -1);
	drawWheel();
	glPopMatrix();

	glPushMatrix();
	glRotatef(20, 1, 0, 0);
	drawSeatTray();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -3, 4);
	drawWheel();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, -0.25);
	glScalef(0.75, 0.75, 0.75);
	drawTubes();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 1.45);
	glRotatef(-45, 1, 0, 0);
	glScalef(1, 1, 1);
	drawSeatTray();
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -1.5);
	drawHandleBar();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.3, 0.2, 0.4);
	glTranslatef(0, -2, 3.25);
	drawSeat();
	glPopMatrix();
}


//Set Properties of light sources and materials
void setLightingandShading() {
	//Enable lighting
	glEnable(GL_LIGHTING); //Tell opengl that we are setting custom lighting

	//Create light source
	glEnable(GL_LIGHT0);
	//Define the illumination or intensity of the light source
	//(r, g, b, opacity) - normalized values in arrays
	GLfloat l0amb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat l0diff[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat l0spec[] = { 1.0, 1.0, 1.0, 1.0 };

	//Define the position
	//(x, y, z, type_of_light_source)
	GLfloat l0pos[] = { 1.0, 1.0, 10.0, 1.0 };
	//GLfloat l0pos[] = { 0, 0.0, 0, 1.0 };

	//Set the illumination (ambiant, diffusion, specular)
	glGetLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glGetLightfv(GL_LIGHT0, GL_DIFFUSE, l0diff);
	glGetLightfv(GL_LIGHT0, GL_SPECULAR, l0spec);

	//Set the position of the light source 0
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);

	//Seting surface colors and properties
	//Enable glColor function apply its color
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	//Set the ambient and diffuse properties
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Set specular intensity
	GLfloat specRef[] = { 0.7, 0.7,0.7, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specRef);

	//Set the specular material property
	GLint shiness = 128;
	glMateriali(GL_FRONT, GL_SHININESS, shiness);
}



void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
	gluLookAt(0.0, 0.0 + camY, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// move the scene (all the rendered environment) using keyboard keys
	glTranslatef(sceTX, sceTY, sceTZ);
	glRotatef(sceRY, 0.0, 1.0, 0.0);

	//Draw a grid on the xz plane
	glColor3f(1, 1, 1);
	if (gridOn == 1)
		drawGrid();
	//draw the three axes
	if (axesOn == 1)
		drawAxes();

	
	drawBicycle();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Define the Perspective projection frustum
	// (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		camY += 1;

	if (key == GLUT_KEY_DOWN)
		camY -= 1;

	if (key == GLUT_KEY_RIGHT)
		sceTX += 1;

	if (key == GLUT_KEY_LEFT)
		sceTX -= 1;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'l')
		objRY += 1;

	if (key == 'r')
		objRY -= 1;

	if (key == 'Z')
		sceTZ += 1;

	if (key == 'z')
		sceTZ -= 1;

	if (key == 'w')
		sceTX += 1;

	if (key == 's')
		sceTX -= 1;

	if (key == 'y')
		sceRY += 1;

	if (key == 'Y')
		sceRY -= 1;

	if (key == '!')
		glDisable(GL_LIGHT0); // Light at -x

	if (key == '1')
		glEnable(GL_LIGHT0);

	if (key == '@')
		glDisable(GL_LIGHT1); // Light at +x

	if (key == '2')
		glEnable(GL_LIGHT1);

	//Grids and axes
	if (key == 'G')
		gridOn = 1;
	if (key == 'g')
		gridOn = 0;
	if (key == 'A')
		axesOn = 1;
	if (key == 'a')
		axesOn = 0;

	glutPostRedisplay();
}

int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Texture Mapping-Defined pattern");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}





// =====================================================================
/*

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

//variables to move the camera
GLfloat camX = 0.0; GLfloat camY = 0.0; GLfloat camZ = 0.0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

////variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

float dollPosition[3] = { 0.0f, 0.0f, 0.0f };
float dollRotation = 0.0f;

static float dRot;


//Grid and coordinate axes for better visualization
void drawGrid() {
GLfloat step = 1.0f;
GLint line;

glBegin(GL_LINES);
for (line = -20; line <= 20; line += step) {
glVertex3f(line, -0.4, 20);
glVertex3f(line, -0.4, -20);

glVertex3f(20, -0.4, line);
glVertex3f(-20, -0.4, line);
}
glEnd();
}

void drawAxes() {

glBegin(GL_LINES);

glLineWidth(1.5);

glColor3f(1.0, 0.0, 0.0);
glVertex3f(-200, 0, 0);
glVertex3f(200, 0, 0);

glColor3f(0.0, 1.0, 0.0);
glVertex3f(0, -200, 0);
glVertex3f(0, 200, 0);

glColor3f(0.0, 0.0, 1.0);
glVertex3f(0, 0, -200);
glVertex3f(0, 0, 200);

glEnd();
}


void init(void) {
glClearColor(0.701, 0.698, 0.996, 1.0);
glClearDepth(1.0);
glEnable(GL_DEPTH_TEST);
}


void setMaterial(float r, float g, float b, float shininess) {
GLfloat materialColor[] = { r, g, b, 1.0f };
glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void BellyCoat() {
GLUquadric* quad = gluNewQuadric();

glPushMatrix();
glTranslatef(0, 0.5, 0);
glRotatef(90.0, 1, 0, 0);
glScalef(1, 0.7, 1);
glColor3f(0.996, 0.376, 0.007);
gluCylinder(quad, 0.6, 0.8, 1.0, 100, 100);
glPopMatrix();

gluDeleteQuadric(quad);
}

void Head() {
GLUquadric* quad = gluNewQuadric();

glPushMatrix();
glTranslatef(0.0, 1.0, 0.0);
//glRotatef(-45.0, 0.0f, 0.0f, 1.0f);
glColor3f(0.815, 0.647, 0.482);
//glScalef(1.0f, 0.5f, 1.0f);
gluSphere(quad, 0.8, 20, 20);
glPopMatrix();

gluDeleteQuadric(quad);
}

void Eyes() {
GLUquadric* quad = gluNewQuadric();

// Left Eye
glPushMatrix();
glTranslatef(0.21, 1.0, 0.78);
glRotatef(-45.0, 0.0f, 0.0f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f); // White
glScalef(2.5f, 1.7f, 0.5f);
gluSphere(quad, 0.1, 20, 20);
glPopMatrix();

// Right Eye
glPushMatrix();
glTranslatef(-0.21, 1.0, 0.78);
glRotatef(45.0, 0.0f, 0.0f, 1.0f);
glColor3f(1.0f, 1.0f, 1.0f); // White
glScalef(2.5f, 1.7f, 0.5f);
gluSphere(quad, 0.1, 20, 20);
glPopMatrix();

gluDeleteQuadric(quad);
}

void Pupils() {
glPushMatrix();
glTranslatef(0.21, 1.0, 0.8);
glColor3f(0.0f, 0.0f, 0.0f); // Black
GLUquadric* quad = gluNewQuadric();
gluSphere(quad, 0.05, 20, 20);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.21, 1.0, 0.8);
gluSphere(quad, 0.05, 20, 20);
glPopMatrix();

gluDeleteQuadric(quad);
}

void Arms() {
glPushMatrix();
glTranslatef(0.8, 0.5, 0);
glScalef(0.2, 0.5, 0.2);
GLUquadric* quad = gluNewQuadric();
glColor3f(1.0f, 0.647f, 0.0f);
gluCylinder(quad, 0.5, 0.5, 20.0, 10, 20);
gluDeleteQuadric(quad);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.8, 0.5, 0);
glScalef(0.2, 0.5, 0.2);
quad = gluNewQuadric();
glColor3f(1.0f, 0.647f, 0.0f);
gluCylinder(quad, 0.5, 0.5, 1.0, 100, 100);
gluDeleteQuadric(quad);
glPopMatrix();
}

void drawDoll() {

BellyCoat();
Head();
Eyes();
Pupils();
Arms();
}

//Set Properties of light sources and materials
void setLightingandShading() {
//Enable lighting
glEnable(GL_LIGHTING); //Tell opengl that we are setting custom lighting

//Create light source
glEnable(GL_LIGHT0);
//Define the illumination or intensity of the light source
//(r, g, b, opacity) - normalized values in arrays
GLfloat l0amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat l0diff[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat l0spec[] = { 1.0, 1.0, 1.0, 1.0 };

//Define the position
//(x, y, z, type_of_light_source)
GLfloat l0pos[] = { 1.0, 1.0, 10.0, 1.0 };
//GLfloat l0pos[] = { 0, 0.0, 0, 1.0 };

//Set the illumination (ambiant, diffusion, specular)
glGetLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
glGetLightfv(GL_LIGHT0, GL_DIFFUSE, l0diff);
glGetLightfv(GL_LIGHT0, GL_SPECULAR, l0spec);

//Set the position of the light source 0
glLightfv(GL_LIGHT0, GL_POSITION, l0pos);

// Set the  light 1 properties (warm white light)
GLfloat l1amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat l1diff[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat l1spec[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat l1pos[] = { 1.0, 1.0, -10.0, 1.0 };

glLightfv(GL_LIGHT1, GL_AMBIENT, l1amb);
glLightfv(GL_LIGHT1, GL_DIFFUSE, l1diff);
glLightfv(GL_LIGHT1, GL_SPECULAR, l1spec);
glLightfv(GL_LIGHT1, GL_POSITION, l1pos);

//Seting surface colors and properties
//Enable glColor function apply its color
glEnable(GL_COLOR_MATERIAL);
glShadeModel(GL_SMOOTH);

//Set the ambient and diffuse properties
glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


//Set specular intensity
GLfloat specRef[] = { 0.7, 0.7,0.7, 1.0 };
glMaterialfv(GL_FRONT, GL_SPECULAR, specRef);

//Set the specular material property
GLint shiness = 128;
glMateriali(GL_FRONT, GL_SHININESS, shiness);
}


void display(void) {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glPushMatrix();
// camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
gluLookAt(0.0, 1.0 + camY, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

// move the scene (all the rendered environment) using keyboard keys
glTranslatef(sceTX, sceTY, sceTZ);
glRotatef(sceRY, 0.0, 1.0, 0.0);


//Draw a grid on the xz plane
glColor3f(1, 1, 1);
drawGrid();
//draw the three axes
drawAxes();

glPushMatrix();
glScalef(2.0f, 2.0f, 2.0f);
drawDoll();
glPopMatrix();

setLightingandShading();

glPopMatrix();
glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
glViewport(0, 0, w, h);
GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

//Define the Perspective projection frustum
// (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

void keyboardSpecial(int key, int x, int y) {
if (key == GLUT_KEY_UP)
camY += 1;

if (key == GLUT_KEY_DOWN)
camY -= 1;

if (key == GLUT_KEY_RIGHT)
sceTX += 1;

if (key == GLUT_KEY_LEFT)
sceTX -= 1;

glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
//ifs can be replaced with switch...case
if (key == 'o')
if (dRot != 120)
dRot += 2;
if (key == 'c')
if (dRot != 0)
dRot -= 2;

if (key == 'l')
objRY += 1;

if (key == 'r')
objRY -= 1;

if (key == 'Z')
sceTZ += 0.2;

if (key == 'z')
sceTZ -= 0.2;

if (key == 'w')
sceTX += 1;

if (key == 's')
sceTX -= 1;

if (key == 'y')
sceRY += 1;

if (key == 'Y')
sceRY -= 1;

if (key == '1')
glEnable(GL_LIGHT0);

if (key == '!')
glDisable(GL_LIGHT0);


if (key == '2')
glEnable(GL_LIGHT1);

if (key == '@')
glDisable(GL_LIGHT1);

if (key == '3')
glShadeModel(GL_FLAT);

if (key == '4')
glShadeModel(GL_SMOOTH);



glutPostRedisplay();
}

int main(void) {
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(400, 400);
glutInitWindowPosition(0, 0);
glutCreateWindow("3D Doll");
glutKeyboardFunc(keyboard);
glutSpecialFunc(keyboardSpecial);
glutDisplayFunc(display);
glutReshapeFunc(reshape);
init();
glutMainLoop();
return 0;
}

*/