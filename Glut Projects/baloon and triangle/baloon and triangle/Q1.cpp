/*********
CTIS164 - Template Source Program
----------
STUDENT :
SECTION :
HOMEWORK:
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 400

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

//shapes
#define RECTANGLE 0
#define TRIANGLE 1
#define BALOON 2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//global varibles for this prgram
bool activeTimer = true;
int mode = TRIANGLE;
int r = 180, g = 172, b = 50; //initial colors of the shapes
int rad = 0;

//3 dor names 20 for char number
char shapes[3][20] = { "RECTANGLE","TRIANGLE","BALOON" };
int cool = 3;
int xB = 0, yB = 0;//Baloons intiial positon



void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
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
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void drawRectangle()
{
	glRectf(200, 100, 200,100);

}

void drawTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(xB, yB + 100);
	glVertex2f(xB - 200, yB - 100);
	glVertex2f(xB + 200, yB - 100);

	glEnd();
}

void drawBaloon()
{
	//part-1 string-rope
	glLineWidth(cool);
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(xB, yB - 60);
	glVertex2f(xB, yB - 140-rad);
	glEnd();
	//part 2 body
	glColor3ub(r, g, b);
	circle(xB, yB, rad+70);

	//part 3 tiof the body
	glBegin(GL_TRIANGLES);
	glVertex2f(xB, yB - 50);
	glVertex2f(xB - 15 - rad, yB - 90-rad);
	glVertex2f(xB + 15 + rad, yB - 90-rad);
	glEnd();

	//part4 reflection
	glColor3f(0.9, 0.9, 0.9);
	circle(xB + 30, yB + 30, rad +30);


}

//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(r, g, b);//3ub for initiazi

	switch (mode)
	{
	case RECTANGLE:
		drawRectangle();
		break;
	case TRIANGLE:
		drawTriangle();
		break;
	case BALOON:
		drawBaloon();
		break;
	}

	vprint(-135, -180, GLUT_BITMAP_8_BY_13, "coooll");
	// wiret before this
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
	if (key == 32)
		activeTimer = !activeTimer;
	if (key == 43)
	{
		cool++;
		rad += 10;
	}

	if (key == 45)
	{
		cool--;
		rad -= 10;
	}

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
	if (key == GLUT_KEY_LEFT)
	{
		if (mode == RECTANGLE)
			mode = BALOON;
		else
			mode--;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (mode == BALOON)
			mode = RECTANGLE;
		else
			mode++;

	}
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
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

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
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

	}
	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)
	{
		xB = x - winWidth / 2;
		yB = winHeight / 2 - y;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}
/*******************************************************************************/
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
/*****************************************************************************/
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (mode == BALOON)
		if (activeTimer == 1)
			if (yB < (winHeight / 2) - 70)
				yB++;

	if (mode == TRIANGLE)
		if (activeTimer == 1)
			if (yB - 100 > (-winHeight / 2))
				yB--;




	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("oooo Shaps");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

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

	//time fucntion
	srand(time(0));

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}

