/*********
CTIS164 - Template Source Program
----------
STUDENT : Omer Levent Durdali
SECTION : 3
HOMEWORK: Homework #2
----------
PROBLEMS:
--Cant restart the Game
--No random castesl flying

do cheack for for number of rockets
radnommly random for y axisx to spwan castels
fucky the sşmlutiynly 5 csatel rule
re do restar funvtin
--Thaths it--
*********/

/***************
Points DOES WORK but really hard to cath it
*****************/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>﻿
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD   20 //period for the timer
#define TIMER_ON        1 //0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

						  /* Global Variables for this program */
#define RADIUS 10    //height of cannon
#define HG      4.9f //Half Gravity

#define Wait 0 //Before F1
#define GO 1 // After F1
#define END 2 //Fİnnes

int msecs = 99;
int secs = 19;

float Px = -300,  //position 
Py = 0;     //of the platform

float Cx = -300,  //x and y coordinate
Cy = RADIUS;//of the cannonball

int   Vx = 300;    //horizontal firing speed
float c, h, k;    //parameters of the quadratic equation

bool animation = false; //flag to show if the cannonball is fired/active

float castelX = -180;     //location of the castel
float castelSpd = 3; //castel speed

bool gameover = false; //flag for game status
bool gamestate = Wait; // Game before F1(stop)

int hitx1, hitx2, hity1, hity2;
int point[2] = { 0 };
int pass = 0;//number of catles passed
int passCo = 0;//number of shots
int top;
int go = 0;

void circle(int x, int y, int r) {
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r) {
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font) {
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

/*****
to display text with variables
vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
*****/
void vprint(int x, int y, void *font, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

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
	for (i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void drawGradient(int x1, int y1, int w, int h, float r, float g, float b) {
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glColor3f(r + 0.4, g + 0.4, b + 0.4);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
}

void drawBackground() {
	drawGradient(-500, 300, 1000, 600, 0.5, 0.5, 0.9);

	glColor3f(1, 1, 1);
	print(-150, 275, "-= Hit The moving Castle =-", GLUT_BITMAP_TIMES_ROMAN_24);

	//bottom panel
	glColor3f(0, 0, 0);
	glRectf(-500, -280, 1000, -300);

	//bottom message
	glColor3f(1, 1, 0); //yellow
	print(-450, -295, "F1: Start/Stop,  SpaceBar: Fire/Restart,  Up/Down: Move Luncher", GLUT_BITMAP_HELVETICA_12);

	glColor3f(0, 0, 0);
	glRectf(400, 900, 1000, -280);

	//timer an other stuff
	glColor3f(1, 0, 0);
	vprint(420, 260, GLUT_BITMAP_HELVETICA_18, "TIMER");
	vprint2(410, 240, 0.15, "00:%02d:%02d", secs, msecs);
	vprint(400, 200, GLUT_BITMAP_9_BY_15, "TOTAL POINTS");
	vprint(440, 180, GLUT_BITMAP_9_BY_15, "%d", point[0]);
	vprint(410, 150, GLUT_BITMAP_9_BY_15, "Number of");
	vprint(420, 130, GLUT_BITMAP_9_BY_15, "Castels");
	vprint(440, 110, GLUT_BITMAP_9_BY_15, "%d", pass);
	vprint(410, 70, GLUT_BITMAP_9_BY_15, "Number of");
	vprint(420, 50, GLUT_BITMAP_9_BY_15, "Shots");
	vprint(440, 30, GLUT_BITMAP_9_BY_15, "%d", passCo);
	vprint(405, 0, GLUT_BITMAP_9_BY_15, "Last Point");
	vprint(440, -20, GLUT_BITMAP_9_BY_15, "%d", point[1]);
}

void drawCannon() {
	glColor3f(0, 0, 0);
	circle(Cx - 100, Cy, RADIUS);
}

void drawCofire() {

	glColor3f(0.5, 0.5, 0.5);
	glRectf(-420, Py + 20, -400, Py - 0);
	glColor3f(0.1, 0.1, 0.1);
	glRectf(-500, Py + 25, -420, Py - 5);

	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(-435, Py - 25);
	glVertex2f(-485, Py - 25 + 25);
	glVertex2f(-435, Py - 25 + 25);
	glVertex2f(-485, Py - 25);
	glEnd();

	glColor3f(0.7, 0.7, 0.7);
	circle(-435, Py - 25, RADIUS);
	circle(-485, Py - 25, RADIUS);

	glLineWidth(1);
}

void constructEquation() {
	c = -HG / (Vx * Vx);
	h = Cx;
	k = Cy;
}

/*****
find the y coordinate of the cannonball using its x coordinate,
this is the main quadratic function that represents the path
of the projectile motion.
*****/
float fx(float x) {
	return c * (x - h) * (x - h) + k;
}

/*****
find the x-coordinate of cannonball using its y coordinate
*****/
float fy(float y) {
	return sqrt((y - k) / c) + h;
}

// initial position of the cannonball
void resetCannon() {
	animation = 0;
	Cx = -300;
	Cy = Py + RADIUS;
	//time = 0;
	passCo++;
}

// initial position of Castle
void resetcastel() {
	castelX = -180;
	castelSpd = 3;
	pass++;
}

void drawCastel(int y, int x) {
	glColor3f(0.6, 0.6, 0.1);
	glRectf(0 + x, y + -240, 60 + x, y + -200);

	glColor3f(0.3, 0.3, 0.5);

	hitx1 = -5 + x;
	hitx2 = 12 + x;
	hity1 = y + -240;
	hity2 = y + -180;
	top = y + -170;

	//Point system
	glRectf(-5 + x, y + -240, 12 + x, y + -180);

	glRectf(48 + x, y + -240, 65 + x, y + -180);
	glBegin(GL_TRIANGLES);
	glVertex2f(-5 + x, y + -180);
	glVertex2f(12 + x, y + -180);
	glVertex2f(3.5 + x, y + -170);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(48 + x, y + -180);
	glVertex2f(65 + x, y + -180);
	glVertex2f(56.5 + x, y + -170);
	glEnd();

	glColor3f(0, 0, 0);
	glRectf(20 + x, y + -240, 40 + x, y + -228);
	circle(30 + x, y + -225, 10);
	circle(4.1 + x, y + -200, 5);
	circle(4.1 + x, y + -220, 5);
	circle(56.9 + x, y + -200, 5);
	circle(56.9 + x, y + -220, 5);

}

void drawCastel2(int y, int x) {
	glColor3f(0.6, 0.6, 0.1);
	glRectf(0 + x, y + -240, 60 + x, y + -200);

	glColor3f(0.3, 0.3, 0.5);

	hitx1 = -5 + x;
	hitx2 = 12 + x;
	hity1 = y + -240;
	hity2 = y + -180;
	top = y + -170;

	//Point system
	glRectf(-5 + x, y + -240, 12 + x, y + -180);

	glRectf(48 + x, y + -240, 65 + x, y + -180);
	glBegin(GL_TRIANGLES);
	glVertex2f(-5 + x, y + -180);
	glVertex2f(12 + x, y + -180);
	glVertex2f(3.5 + x, y + -170);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(48 + x, y + -180);
	glVertex2f(65 + x, y + -180);
	glVertex2f(56.5 + x, y + -170);
	glEnd();

	glColor3f(0, 0, 0);
	glRectf(20 + x, y + -240, 40 + x, y + -228);
	circle(30 + x, y + -225, 10);
	circle(4.1 + x, y + -200, 5);
	circle(4.1 + x, y + -220, 5);
	circle(56.9 + x, y + -200, 5);
	circle(56.9 + x, y + -220, 5);

}

void drawCross(int x) {
	glColor3f(1, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(x + 30, -170);
	glVertex2f(x - 30, -210);
	glVertex2f(x + 30, -210);
	glVertex2f(x - 30, -170);
	glEnd();
	glLineWidth(1);
}

/*****
to display onto window using OpenGL commands
*****/
void display() {
	glClearColor(0.8, 0.8, 0.8, 0.8);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	drawCastel(castelX, -150);
	drawCastel2(castelX - 50, -50);
	drawCastel(castelX, 50);

	drawCastel(castelX, 150);

	drawCastel(castelX, 250);

	drawCannon();
	drawCofire();

	//show gameover message 
	if (gameover) {
		glColor3f(1, 1, 0);
		print(-100, 100, "-=- GAME OVER -=-", GLUT_BITMAP_TIMES_ROMAN_24);
		print(-60, 70, "Press Space Bar to continue...", GLUT_BITMAP_HELVETICA_12);
	}

	glutSwapBuffers();
}

/*****
key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
*****/
void onKeyDown(unsigned char key, int x, int y) {
	//exit when ESC is pressed.
	if (key == 27)
		exit(0);

	//if the key is space character.
	if (key == ' ') {
		animation = true;
		//restart the game 
		if (gameover) {
			gameover = false;
			resetCannon();
			resetcastel();
		}
	}

	//to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
	//exit when ESC is pressed.
	if (key == 27)
		exit(0);

	//to refresh the window it calls display() function
	glutPostRedisplay();
}

/*****
special Key like GLUT_KEY_F1, F2, F3,...
arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
*****/
void onSpecialKeyDown(int key, int x, int y) {
	//write your codes here.

	if (key == GLUT_KEY_F1 && gamestate == Wait)
		gamestate = 1;

	if (!animation) {
		switch (key) {
		case GLUT_KEY_UP:
			Py += 5;
			if (Py > 275)
				Py = 275;
			Cy = Py + RADIUS;
			break;
		case GLUT_KEY_DOWN:
			Py -= 5;
			if (Py < -245)
				Py = -245;
			Cy = Py + RADIUS;
			break;
		}

		//construct the new equation, if the vertex is modified
		//due to key presses on arrow keys
		constructEquation();

	}

	//to refresh the window it calls display() function
	glutPostRedisplay();
}

/*****
special Key like GLUT_KEY_F1, F2, F3,...
arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
*****/
void onSpecialKeyUp(int key, int x, int y) {
	//write your codes here

	switch (key) {

	case GLUT_KEY_UP:
		up = false;
		break;

	case GLUT_KEY_DOWN:
		down = false;
		break;

	case GLUT_KEY_LEFT:
		left = false;
		break;

	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	//to refresh the window it calls display() function
	glutPostRedisplay();
}

/*****
When a click occurs in the window,
It provides which button
buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
states  : GLUT_UP , GLUT_DOWN
x, y is the coordinate of the point that mouse clicked
*****/
void onClick(int button, int stat, int x, int y)
{
	//for easier use of  the cordinates
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		printf("Event : CLK %d %d\n", x - winWidth / 2, winHeight / 2 - y);
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

/*****
This function is called when the window size changes.
w : is the new width of the window in pixels
h : is the new height of the window in pixels
*****/
void onResize(int w, int h) {
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); //refresh window
}

void onMoveDown(int x, int y) {
	//write your codes here.
	//to refresh the window it calls display() function   
	glutPostRedisplay();
}

/*****
GLUT to OpenGL coordinate conversion
x2 = x1 - winWidth / 2
y2 = winHeight / 2 - y1
*****/
void onMove(int x, int y) {
	//write your codes here.
	//to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);


	if (gamestate) {

		msecs = msecs - 5 * TIMER_PERIOD / 100;
		//milisecons to seconds for timer
		if (msecs == 00)
		{
			msecs = 99;
			secs = secs - 1;
		}
		if (secs == 00 || msecs == 00)
		{
			gameover = true;
			animation = false;
			gamestate = Wait;

		}
		if (!gameover) {
			if (animation) {
				//move the cannonball in x and y direction
				//new coordinates of the cannonball is
				Cx += 0.02 * Vx;
				Cy = fx(Cx);
				//glRectf(-5 + x, y + -240, 12 + x, y + -180);

				if (Cy == (hity1 + hity2) / 2) {
					if (Cx >= hitx1 && Cx <= hitx2) {
						point[0] = point[0] + 5;
						point[1] = 5;
						resetcastel();
					}
				}

				if (Cy >= hity1 && Cy <= hity2) {
					if (Cx >= hitx1 && Cx <= hitx2) {
						point[0] = point[0] + 3;
						resetcastel();
						point[1] = 3;
					}
				}

				//check if the cannonball goesa out
				if (Cx >= 500) {
					resetCannon();
				}
			}
			//move castel
			castelX += castelSpd;
			if (castelX > 470 || castelX < -190)
				castelSpd = -castelSpd;
		}

		//to refresh the window it calls display() function
		glutPostRedisplay(); //display()
	}


	if (top >= 300) {
		resetcastel();
	}

	//if (go == 100)
	//	drawCastel(castelX, -50);
	//else if (go == 150)
	//	drawCastel(castelX, 50);
	//else if (go == 250)
	//	drawCastel(castelX, 150);
	//else if (go == 350)
	//	drawCastel(castelX, 250);
}
#endif
void Init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	srand(time(NULL));
	bool animation = false;
	bool gamestate = Wait;
	msecs = 99;
	secs = 19;
	constructEquation();
}

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// glutInitWindowPosition(2000, 200);
	glutCreateWindow("Some Game 2");
	Init();

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//keyboard registration
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//mouse registration
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();

}