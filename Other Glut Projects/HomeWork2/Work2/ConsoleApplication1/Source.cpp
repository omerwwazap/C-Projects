/*****
CTIS164 - Lab07
Projectile Motion
*****
ver03: ship added
*****/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
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

float Px = -300,  //position 
Py = 0;     //of the platform

float Cx = -300,  //x and y coordinate
Cy = RADIUS;//of the cannonball

int   Vx = 30;    //horizontal firing speed
float c, h, k;    //parameters of the quadratic equation

bool animation = false; //flag to show if the cannonball is fired/active
float time = 0;

float boatX = 0;     //location of the boat
float boatSpd = 0.5; //boat speed

bool  gameover = false; //flag for game status

						/*****
						to draw circle, center at (x,y) radius r
						DAIRE
						*****/
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

/*****
CEMBER
*****/
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
	for (i = 0; i<len; i++) {
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
	for (i = 0; i<len; i++) {
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
	for (i = 0; i<len; i++) {
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

void drawCloud(int x, int y) {
	glColor3f(1, 1, 1); //white
	circle(0 + x, 0 + y, 30);
	circle(-25 + x, y, 20);
	circle(25 + x, -2 + y, 20);
	circle(21 + x, -19 + y, 10);
}

void drawHArrow(int x1, int x2, int y, int num) {
	glColor3f(0, 0, 0); //black
	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2f(x1, y);
	glVertex2f(x1 + 10, y + 5);

	glVertex2f(x1, y);
	glVertex2f(x1 + 10, y - 5);

	glVertex2f(x1, y);
	glVertex2f(x1 + 100, y);

	glVertex2f(x2, y);
	glVertex2f(x2 - 10, y + 5);

	glVertex2f(x2, y);
	glVertex2f(x2 - 10, y - 5);

	glVertex2f(x2, y);
	glVertex2f(x2 - 100, y);
	glEnd();

	vprint((x1 + x2) / 2, y, GLUT_BITMAP_HELVETICA_12, "%d mt", num);
	glLineWidth(1);
}

void drawVArrow(int y1, int y2, int x, int num) {
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINES);
	glVertex2f(x, y1);
	glVertex2f(x + 5, y1 - 10);

	glVertex2f(x, y1);
	glVertex2f(x - 5, y1 - 10);

	glVertex2f(x, y1);
	glVertex2f(x, y1 - 100);

	glVertex2f(x, y2);
	glVertex2f(x + 5, y2 + 10);

	glVertex2f(x, y2);
	glVertex2f(x - 5, y2 + 10);

	glVertex2f(x, y2);
	glVertex2f(x, y2 + 100);
	glEnd();

	vprint(x + 4, (y1 + y2) / 2, GLUT_BITMAP_HELVETICA_12, "%d mt", num);
	glLineWidth(1);
}

void drawAxis() {
	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_LINES);
	glVertex2f(-400, 0);
	glVertex2f(400, 0);
	glVertex2f(0, 300);
	glVertex2f(0, -300);
	glEnd();
}

void drawBackground() {
	drawGradient(-400, 300, 800, 600, 0.5, 0.5, 0.9); //sky
	drawGradient(-400, -200, 800, 100, 0.2, 0.2, 1); //sea

	drawHArrow(-300, 400, -240, 700);

	drawAxis();

	glColor3f(1, 1, 1);
	print(-310, -215, "x = -300", GLUT_BITMAP_TIMES_ROMAN_10);

	//title
	glColor3f(1, 1, 1);
	print(-90, 275, "-= Projectile Motion =-", GLUT_BITMAP_TIMES_ROMAN_24);

	//bottom info panel
	glColor3f(0, 0, 0);
	glRectf(-400, -280, 400, -300);

	//info message
	glColor3f(1, 1, 0); //yellow
	print(-380, -295, "SpaceBar: Fire/Restart,  Up/Down: Platform height,  Left/Right: Increase/Decrease speed", GLUT_BITMAP_HELVETICA_12);

	//draw 3 clouds
	drawCloud(-250, 180);
	drawCloud(250, 100);
	drawCloud(0, 200);
}

void drawPlatform(int y) {
	drawGradient(-400, y, 100, y + 200, 0.9, 0.1, 0.1);
	drawVArrow(y, -200, -350, (y + 200));
}

/**************************************************/
/* The code written below in the lab starts here  */
/**************************************************/

void drawCannon() {
	glColor3f(0, 0, 0);
	circle(Cx, Cy, RADIUS);
}

void drawSpeed() {
	glColor3f(1, 1, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(Cx, Cy);
	glVertex2f(Cx + Vx, Cy);
	glLineWidth(2);
	glVertex2f(Cx + Vx - 10, Cy - 5);
	glVertex2f(Cx + Vx, Cy);
	glVertex2f(Cx + Vx - 10, Cy + 5);
	glVertex2f(Cx + Vx, Cy);
	glEnd();
	circle(Cx, Cy, 5);
	glLineWidth(1.0);
	char str[30];
	sprintf(str, "Speed : %d m/s", Vx);
	print(-380, 280, str, GLUT_BITMAP_HELVETICA_12);
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

/*****
Show parabola or path
*****/
void drawPath() {
	float x, y;
	glColor3f(0.4, 0.4, 0.9);
	glBegin(GL_LINE_STRIP);
	for (x = -300; x<400; x += 10) {
		y = fx(x);
		if (y > -200)
			glVertex2f(x, y);
	}
	x = fy(-200);
	glVertex2f(x, -200);
	glEnd();

	//x is the point that the cannonball hits the surface of the sea
	circle(x, -200, 5);
	char str[30];
	sprintf(str, "x = %.1f", x);
	glColor3f(1, 1, 1);
	print(x, -215, str, GLUT_BITMAP_TIMES_ROMAN_10);
}

// initial position of the cannonball.
void resetCannon() {
	animation = 0;
	Cx = -300;
	Cy = Py + RADIUS;
	time = 0;
}

// initial position of boat.
void resetBoat() {
	boatX = 0;
	boatSpd = 0.5;
}

void drawBoat(int x) {
	glColor3f(0.6, 0.6, 0.1);
	glRectf(x, -210, x + 60, -190);
	glColor3f(0.3, 0.3, 0.3);
	glRectf(x + 15, -190, x + 18, -160);
	glColor3f(0.9, 0.9, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 18, -190);
	glVertex2f(x + 18, -160);
	glVertex2f(x + 48, -180);
	glEnd();
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
	drawPlatform(Py);
	drawBoat(boatX);
	drawCannon();

	//show speed arrow only when the cannon is not moving
	if (!animation) {
		drawSpeed();
	}

	drawPath();

	//show gameover message and a cross on the boat
	if (gameover) {
		glColor3f(1, 1, 0);
		print(-100, 100, "-=- GAME OVER -=-", GLUT_BITMAP_TIMES_ROMAN_24);
		print(-60, 70, "Press Space Bar to continue...", GLUT_BITMAP_HELVETICA_12);
		drawCross(boatX + 30);
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
		//restart the game if the game is already over
		if (gameover) {
			gameover = false;
			resetCannon();
			resetBoat();
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

	if (!animation) {

		switch (key) {
		case GLUT_KEY_RIGHT:
			Vx += 5;
			if (Vx > 120)
				Vx = 120;
			break;

		case GLUT_KEY_LEFT:
			Vx -= 5;
			if (Vx < 15)
				Vx = 15;
			break;

		case GLUT_KEY_UP:
			Py += 5;
			if (Py > 200)
				Py = 200;
			Cy = Py + RADIUS;
			break;

		case GLUT_KEY_DOWN:
			Py -= 5;
			if (Py < -50)
				Py = -50;
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
void onClick(int button, int stat, int x, int y) {
	//write your codes here.



	//to refresh the window it calls display() function
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

	if (!gameover) {
		if (animation) {
			//move the cannonball in x and y direction
			//new coordinates of the cannonball is
			Cx += 0.02 * Vx;
			Cy = fx(Cx);

			//check if the cannonball hits the surface of the sea
			if (Cy < (-200 + RADIUS)) {
				//collision detection part:
				//check if cannonball hits the boat
				if (Cx >= boatX && Cx <= (boatX + 60)) {
					gameover = true;
				}
				else {
					//cannonball did not hit the boat, retry
					resetCannon();
				}
			}
		}

		//move boat
		boatX += boatSpd;
		if (boatX > 340 || boatX < -210)
			boatSpd = -boatSpd;
	}

	//to refresh the window it calls display() function
	glutPostRedisplay(); //display()

}
#endif

void init() {
	//smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//at the beginning, find the parameters of the quadratic equation
	constructEquation();
}

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// glutInitWindowPosition(2000, 200);
	glutCreateWindow("Lab07(800x600) Projectile Motion");

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

	init();

	glutMainLoop();

}
