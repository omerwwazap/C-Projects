
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define PI 3.1415

#define NUM_BALLS 6

typedef struct {
	float x, y;
} vector2D;

typedef struct {
	vector2D vel;
	vector2D pos;
	float r;
	bool col;
	int red, green, blue;
} ball_t;

ball_t balls[NUM_BALLS] = { { { 2, 0 } ,{ -200, 30 }, 40 },
{ { -4, 0 } ,{ 200, 0 }, 40 }
};

int width, height, halfW, halfH;

float length(vector2D v) {
	float d = sqrt(v.x * v.x + v.y * v.y);
	return d;
}

vector2D  findVector(vector2D v1, vector2D v2) {
	vector2D v;
	v.x = v2.x - v1.x;
	v.y = v2.y - v1.y;
	return v;
}

float dotProduct(vector2D v1, vector2D v2) {
	float d = v1.x * v2.x + v1.y * v2.y;
	return d;
}

vector2D addVector(vector2D v1, vector2D v2) {
	vector2D v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return v;
}

bool collision(ball_t b1, ball_t b2) {
	vector2D v = findVector(b1.pos, b2.pos);
	float d = length(v);
	if (d <= b1.r + b2.r) {
		return true;
	}
	return false;
}

vector2D scalarProduct(float k, vector2D v1) {
	vector2D v;
	v.x = k * v1.x;
	v.y = k * v1.y;
	return v;
}


//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
	float angle;

	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

void displayBalls(ball_t b) {
	glColor3f(1, 1, 1);
	circle(b.pos.x, b.pos.y, b.r);
	glColor3ub(b.red, b.green, b.blue);
	circle(b.pos.x, b.pos.y, b.r - 3);
}


//
// To display onto window using OpenGL commands
//
void display()
{
	int i;
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	for (i = 0; i< NUM_BALLS; i++) {
		displayBalls(balls[i]);
	}
	glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.

	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.

	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void reshape(int w, int h)
{
	width = w;
	height = h;
	halfW = w / 2;
	halfH = h / 2;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function	
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void collisonTwoBalls(int i, int j) {
	if (collision(balls[i], balls[j])) {

		vector2D u1 = balls[i].vel;
		vector2D u2 = balls[j].vel;

		vector2D w = findVector(balls[i].pos, balls[j].pos);
		float   len_w = length(w);

		// first object velocity components
		vector2D u1x = scalarProduct(dotProduct(w, u1) / (len_w * len_w), w);
		vector2D u1y = addVector(u1, scalarProduct(-1, u1x));

		// second object velocity components
		vector2D u2x = scalarProduct(dotProduct(w, u2) / (len_w * len_w), w);
		vector2D u2y = addVector(u2, scalarProduct(-1, u2x));

		float m1 = balls[i].r;
		float m2 = balls[j].r;
		float totalm = m1 + m2;

		vector2D v1x, v2x;
		v1x = addVector(scalarProduct((m1 - m2) / totalm, u1x), scalarProduct(2 * m2 / totalm, u2x));
		v2x = addVector(scalarProduct((m2 - m1) / totalm, u2x), scalarProduct(2 * m1 / totalm, u1x));

		vector2D v1, v2;
		v1 = addVector(v1x, u1y);
		v2 = addVector(v2x, u2y);

		//balls[i].pos = addVector( balls[i].pos, scalarProduct(-1,balls[i].vel) ) ; 
		//balls[j].pos = addVector( balls[j].pos, scalarProduct(-1,balls[j].vel) ) ; 

		balls[i].vel = v1;
		balls[j].vel = v2;

	}
}

void checkCorners(int i) {
	float right = halfW - balls[i].r;
	float top = halfH - balls[i].r;
	float bottom = -top;
	float left = -right;

	if (balls[i].pos.x > right) {
		balls[i].pos.x = right;
		balls[i].vel.x *= -1;
	}

	if (balls[i].pos.x < left) {
		balls[i].pos.x = left;
		balls[i].vel.x *= -1;
	}

	if (balls[i].pos.y > top) {
		balls[i].pos.y = top;
		balls[i].vel.y *= -1;
	}

	if (balls[i].pos.y < bottom) {
		balls[i].pos.y = bottom;
		balls[i].vel.y *= -1;
	}

}

void onTimer(int v) {
	int i, j;
	glutTimerFunc(20, onTimer, 0);
	// Write your codes here.

	for (i = 0; i<NUM_BALLS - 1; i++) {
		for (j = i + 1; j<NUM_BALLS; j++) {
			collisonTwoBalls(i, j);
		}
	}

	//update balls
	for (i = 0; i< NUM_BALLS; i++) {
		checkCorners(i);
		balls[i].pos = addVector(balls[i].pos, balls[i].vel);
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
void main(int argc, char *argv[])
{
	float theta = 2 * PI / NUM_BALLS;
	srand(time(NULL));
	for (int i = 0; i < NUM_BALLS; i++) {

		// Balls are positioned on a circle.
		balls[i].pos.x = 200 * cos(i * theta);
		balls[i].pos.y = 200 * sin(i * theta);

		// random ball Mass or size
		balls[i].r = (rand() % 50) + 10;

		// random ball velocity
		balls[i].vel.x = ((rand() % 10) - 5) + 1;
		balls[i].vel.y = ((rand() % 10) - 5) + 1;

		// Random ball color
		balls[i].red = rand() % 256;
		balls[i].green = rand() % 256;
		balls[i].blue = rand() % 256;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Template File");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

	// timer event
	glutTimerFunc(20, onTimer, 0);

	glutMainLoop();
}