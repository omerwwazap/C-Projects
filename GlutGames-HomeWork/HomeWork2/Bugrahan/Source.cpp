/***
CTIS164 - Template Source Program
----------
STUDENT :
SECTION :
HOMEWORK:
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
***/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define MAX_FIRE 25
#define FIRE_RATE 5

//game states
#define LOAD//add pls
#define STOP 0
#define START 1

#define D2R 0.0174532
int timecnt;
int i;
int gameState;
int hit;
bool fire;
int asd = 0;
int firingRate = 0;
//random numbers (purpose is to generate random colors in each run)
int random1;
int random2;
int random3;

typedef struct {
	int x, y;
}point_t;

point_t pos;

typedef struct {
	int x, y;
	point_t pos;
	int speed;
	double angle;
	bool active;
}fire_t;

fire_t fires[MAX_FIRE];

typedef struct {
	int x, y;
	point_t pos;
	double angle;
}target_t;

target_t targets[3];

typedef struct {
	double angle;
	point_t pos;
	int lenght;
	double   r;
}player_t;

player_t player = { 0,200 };

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //
void initialize()
{
	int k;
	srand(time(NULL));
	for (k = 0; k < 3; k++)
	{
		targets[k].angle = rand() % 360;
		targets[k].x = sin(targets[k].angle)*(100 * (k + 1) + 50);
		targets[k].y = cos(targets[k].angle)*(100 * (k + 1) + 50);
	}
}

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

void displayer()
{
	glColor3f(0, 0, 1);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(sin(player.angle)* player.lenght, cos(player.angle)* player.lenght);
	circle(0, 0, 10);
	//Alien writings
	glColor3f(0, 1, 0);
	vprint(50, 50, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.1);
	vprint(40, 50, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.2);
	vprint(30, 50, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.3);
	vprint(50, 40, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.4);
	vprint(40, 40, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.5);
	vprint(30, 40, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.6);
	vprint(50, 30, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.7);
	vprint(40, 30, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.8);
	vprint(30, 30, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.9);
	vprint(40, 20, GLUT_BITMAP_8_BY_13, "%c", targets[0].angle + 0.01);
}

void displayOrbit()
{
	glColor3f(1, 1, 1);
	glLineWidth(1);
	for (i = 0; i < 3; i++)
		circle_wire(0, 0, 100 * (i + 1) + 50);
}

void displayTargets()
{
	int i;
	
	glColor3ub(random1, random2, random3);
	for (i = 0; i < 3; i++)
	{
		targets[i].x = sin(targets[i].angle)*(100 * (i + 1) + 50);
		targets[i].y = cos(targets[i].angle)*(100 * (i + 1) + 50);
		circle(targets[i].x, targets[i].y, 15);
	}

}

int findAvailableFire() {
	for (int i = 0; i < MAX_FIRE; i++) {
		if (fires[i].active == false)
			return i;
	}
	return -1;
}

void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	displayOrbit();
	displayer();
	displayTargets();



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
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

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
	// Write your codes here.
	if (button == GLUT_LEFT_BUTTON)
		gameState = START;
	if (button == GLUT_LEFT_BUTTON && gameState == START)
		fire = 1;

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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	x = x - winWidth / 2;
	y = winHeight / 2 - y;

	player.angle = atan2(x, y);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	srand(time(NULL));
	//making the targets move with random speeds
	if (gameState == START)
	{
		timecnt++;//time counter
		for (i = 0; i < 3; i++)
		{
			targets[i].angle += 0.03;
		}
	}
	asd++;
	if (fire && asd % FIRE_RATE < 10)
	{
		int free = findAvailableFire();
		if (free != -1)
		{
			fires[free].pos = player.pos;
			fires[free].angle = player.angle;
			fires[free].active = true;
			firingRate = FIRE_RATE;
		}
	}

	// Move all fires that are active.
	for (int i = 0; i< MAX_FIRE; i++)
	{
		if (fires[i].active)
		{
			fires[i].pos.x += 10 * cos(fires[i].angle * D2R);
			fires[i].pos.y += 10 * sin(fires[i].angle * D2R);

			if (fires[i].pos.x > 350 || fires[i].pos.x < -350 || fires[i].pos.y > 350 || fires[i].pos.y < -350)
			{
				fires[i].active = false;
			}
		}
	}

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
	glutCreateWindow("Alien Orbit Shooter by Bugrahan Durukan");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//random color generation for targets
	srand(time(NULL));
	random1 = rand() % 255;
	random2 = rand() % 255;
	random3 = rand() % 255;
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

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}