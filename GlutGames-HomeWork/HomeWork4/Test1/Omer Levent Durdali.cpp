/***
CTIS164 - Template Source Program
----------
STUDENT : 
SECTION : 03
HOMEWORK: #4
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
-- One more Light source -> Yellow
***/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"

#define LightNUM 5 //DO NOT CHANGE

#define PlanetNUM 3//CAN BE CAHNGED 
//After 100 it becomes intersting and fun

#define PI 3.1415

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 715

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define PLAY 0
#define STOP 1
#define LightSIZE 10
#define SUNSIZE 30

int state = PLAY;

int Testing = 0;//if 0 starts with planets 
				//if 1 starts with no planest entil reset button is hit

int red = 1, green = 1, blue = 1, sun = 1, yellow = -1;//Light source initilize


													   /* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //
typedef struct {
	float r, g, b;
} color_t;

typedef struct {
	vec_t   pos;
	color_t color;
	vec_t   vel;
} light_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;

typedef struct {
	vec_t pos;
	int speed, turn = 1, distance;
	double angle;
	double radius;
}planets_t;


planets_t Planet[PlanetNUM];


light_t light[LightNUM]
{
	{ {},{ 1, 0, 0 },{ 4,  2 } },//red
	{ {},{ 0, 1, 0 },{ -3, -1 } },//green
	{ {},{ 0, 0, 1 },{ 5, -2 } },//blue
	{ { 0, 0 },{ 1, 1, 1 },{ 0, 0 } },//The SUN
	{ {},{ 1, 1, 0 },{ 5, -2 } },//yellow 

};

void drawbackground();


color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

// To add distance into calculation
// when distance is 0  => its impact is 1.0
// when distance is 350 => impact is 0.0
// Linear impact of distance on light calculation.
double distanceImpact(double d) {
	return (-1.0 / 350.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = dotP(uL, v.N) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
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

//The ultimate cirlce function -> with some help from Stackoverflow 
void circlefan(int x, int y, int r) {

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.3, 0.3, 0.3, 0.9);
	glVertex2f(x, y);//triangle center
	for (int i = 0; i <= 100; i++)
	{
		float angle;
		angle = 2 * PI*i / 100;
		vec_t center = { x, y };
		vec_t point = { x + r*cos(angle) , y + r*sin(angle) };
		vec_t WOWitworks = subV(point, center);

		double x1 = WOWitworks.x, y1 = WOWitworks.y;
		vertex_t Points = { { x + r*cos(angle), y + r*sin(angle) },{ x1, y1 } };

		color_t Norm = { 1, 1, 1 };

		for (int i = 0; i < LightNUM; i++) {
			//without cheacking ( I ) it calculates them all
			//cheacking color becasue if its false it will add so cheack needed
			if (i == 0 && red == 1)
				Norm = addColor(Norm, calculateColor(light[i], Points));
			if (i == 1 && green == 1)
				Norm = addColor(Norm, calculateColor(light[i], Points));
			if (i == 2 && blue == 1)
				Norm = addColor(Norm, calculateColor(light[i], Points));
			if (i == 3 && sun == 1)
				Norm = addColor(Norm, calculateColor(light[i], Points));
			if (i == 4 && yellow == 1)
				Norm = addColor(Norm, calculateColor(light[i], Points));
		}

		glColor3f(Norm.r / (double)15, Norm.g / (double)15, Norm.b / (double)15);
		//res is float/double 
		//Calculations of res is to big so needed to be diveded

		glVertex2f(x + r*cos(angle), y + r*sin(angle));//tirnagle location and angle
	}
	glEnd();
}
//**************************************************************************
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

void drawbackground() {
	glColor3f(0, 1, 0);
	vprint(-330, 325, GLUT_BITMAP_9_BY_15, "HOMEWORK #4");
	vprint(-330, 305, GLUT_BITMAP_9_BY_15, "Levent Durdali");
	vprint(-330, 285, GLUT_BITMAP_9_BY_15, "21702600");

	glColor3f(0.1, 0.1, 0.1);
	glRectf(350, -370, -350, -320);

	//******
	if (red == 1) {
		glColor3f(1, 0, 0);
		vprint(-330, -335, GLUT_BITMAP_8_BY_13, "F1(Red) = ON");
	}
	else {
		glColor3f(1, 1, 1);
		vprint(-330, -335, GLUT_BITMAP_8_BY_13, "F1(Red) = OFF");
	}
	//******
	if (green == 1) {
		glColor3f(0, 1, 0);
		vprint(-220, -335, GLUT_BITMAP_8_BY_13, "F2(Green) = ON");
	}
	else {
		glColor3f(1, 1, 1);
		vprint(-220, -335, GLUT_BITMAP_8_BY_13, "F2(Green) = OFF");
	}
	//******
	if (blue == 1) {
		glColor3f(0, 0, 1);
		vprint(-100, -335, GLUT_BITMAP_8_BY_13, "F3(Blue):ON");
	}
	else {
		glColor3f(1, 1, 1);
		vprint(-100, -335, GLUT_BITMAP_8_BY_13, "F3(Blue):OFF");
	}
	//******
	if (sun == 1) {
		glColor3f(1, 1, 0);
		vprint(0, -335, GLUT_BITMAP_8_BY_13, "F4(Sun):ON");
	}
	else {
		glColor3f(1, 1, 1);
		vprint(0, -335, GLUT_BITMAP_8_BY_13, "F4(Sun):OFF");
	}
	//*******
	if (yellow == 1) {
		glColor3f(1, 1, 0);
		vprint(0, -350, GLUT_BITMAP_8_BY_13, "Space(Yellow):ON");
	}
	else {
		glColor3f(1, 1, 1);
		vprint(0, -350, GLUT_BITMAP_8_BY_13, "Space(Yellow):OFF");
	}

	//Animations Stop/Play
	glColor3f(1, 1, 1);
	if (state == PLAY)
		vprint(100, -335, GLUT_BITMAP_8_BY_13, "F5(Animation):ON");
	else
		vprint(100, -335, GLUT_BITMAP_8_BY_13, "F5(Animation):OFF");

	//Game restart
	vprint(255, -335, GLUT_BITMAP_8_BY_13, "F6:Restart");
}

void display() {
	//
	// clear window to black
	//
	srand(time(NULL));
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	for (int i = 0; i < LightNUM; i++) {

		//colors for light soruce
		glColor3f(light[i].color.r, light[i].color.g, light[i].color.b);


		//Showing and not showing light sources
		if (i == 3 && sun == 1)
			circle(light[i].pos.x, light[i].pos.y, SUNSIZE);
		else if (i == 0 && red == 1)
			circle(light[i].pos.x, light[i].pos.y, LightSIZE);
		else if (i == 1 && green == 1)
			circle(light[i].pos.x, light[i].pos.y, LightSIZE);
		else if (i == 2 && blue == 1)
			circle(light[i].pos.x, light[i].pos.y, LightSIZE);
		else if (i == 4 && yellow == 1)
			circle(light[i].pos.x, light[i].pos.y, LightSIZE);
	}

	for (int i = 0; i < PlanetNUM; i++)
	{
		// coordinats on move
		Planet[i].pos.x = Planet[i].distance * cos(Planet[i].angle * D2R);
		Planet[i].pos.y = Planet[i].distance * sin(Planet[i].angle * D2R);
		circlefan(Planet[i].pos.x, Planet[i].pos.y, Planet[i].radius);//Color function
	}

	drawbackground();

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
	else if (key == ' ')
		yellow *= -1;
	


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
	if (key == GLUT_KEY_F1)
		red *= -1;
	else if (key == GLUT_KEY_F2)
		green *= -1;
	else if (key == GLUT_KEY_F3)
		blue *= -1;
	else if (key == GLUT_KEY_F4)
		sun *= -1;
	else if (key == GLUT_KEY_F5 && state == PLAY)
		state = STOP;
	else if (key == GLUT_KEY_F5 && state == STOP)
		state = PLAY;
	else if (key == GLUT_KEY_F6)
		Testing = 0;//resets everthing


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



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	srand(time(NULL));

	//restaring option
	if (Testing == 0)
	{

		//Planet[2]
		for (int k = 0; k < PlanetNUM; k++)
		{
			int bettingT;
			bettingT = rand() % 2;
			Planet[k].radius = rand() % 20 + 10; // random radius
			Planet[k].angle = rand() % 360; // random variables
			Planet[k].speed = rand() % 2 + 1; // movement
			Planet[k].distance = rand() % 150 + 150; // distance between planets and the Sun


			if (bettingT == 0)//Clockwise/counter-clockwise rotation
				Planet[k].turn *= -1;
			else
				Planet[k].turn = 1;
		}

		//when reset these change the cordniants of the light sources
		for (int a = 0; a < 3; a++)
		{
			light[a].pos.x = rand() % 500 - 350;//x value margins
			light[a].pos.y = rand() % 350;// if than bigger this it sometimes goes of out screen
										  //so this stops it
		}

		Testing = 1;
	}

	//make Light locaitons always inside screen
	//same as lab fuction
	for (int i = 0; i < LightNUM; i++) {
		if (state == PLAY)
			light[i].pos = addV(light[i].pos, light[i].vel);

		if (light[i].pos.x > 350 || light[i].pos.x < -350) {
			light[i].vel.x *= -1;
		}

		if (light[i].pos.y > 350 || light[i].pos.y < -310) {
			light[i].vel.y *= -1;
		}
	}


	for (int j = 0; j < PlanetNUM; j++)
	{
		if (state == PLAY)//speed
			Planet[j].angle += Planet[j].speed *Planet[j].turn;

		//orbits
		if (Planet[j].angle < 0)
			Planet[j].angle += 360;
		else if (Planet[j].angle > 360)
			Planet[j].angle -= 360;
	}



	// to refresh the window it calls display() function
	glutPostRedisplay();

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
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Solar System 100 Planet Simulator ");

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

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}