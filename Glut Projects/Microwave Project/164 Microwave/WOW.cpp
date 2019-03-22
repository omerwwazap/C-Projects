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

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

//States of the progran
#define START 0
#define RUN 1
#define END 2
#define OPEN 3

#define DURATION 10

int appstate = START;

bool inStartButton = false;
int timeCounter = DURATION;

//initilize the timer function so it works sirlamasi olsun siye
void onTimer(int v);

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //
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

bool checkCircle(float px, float py,//of the point
	float cx, float cy, //center of the cirlce
	float r)
{
	float dx = px - cx;
	float dy = py - cy;
	float d = sqrt(dx*dx + dy*dy);
	return(d <= r);


}


void diplayBackGround()
{
	//label

	glColor3f(0, 0, 0);
	vprint(-240, 230, GLUT_BITMAP_8_BY_13, "Name Surname ");
	vprint(-240, 210, GLUT_BITMAP_8_BY_13, "21702600");
	vprint(-70, 200, GLUT_BITMAP_9_BY_15, "- HomeWork -");

	//main container

	glColor3ub(225, 230, 255);
	glRectf(-200, -150, 200, 150);

	glColor3ub(255, 255, 255);
	glRectf(-200, -150, 200, 91);

	glColor3ub(100, 130, 180);
	glRectf(-200, -150, 200, -92);

	glLineWidth(3);
	glColor3ub(140, 170, 200);
	glBegin(GL_LINES);
	glVertex2f(-200, 90);
	glVertex2f(200, 90);
	glEnd();

	glColor3ub(90, 100, 140);
	glBegin(GL_LINES);
	glVertex2f(-200, -91);
	glVertex2f(200, -91);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glVertex2f(-200, -150);
	glVertex2f(-200, 150);
	glVertex2f(200, 150);
	glVertex2f(200, -150);
	glEnd();

	glLineWidth(1);

	vprint(-30, 120, GLUT_BITMAP_9_BY_15, "Microwave Inteface V1.0");

}
void displayStart()
{
	glColor3ub(90, 100, 140);
	circle(0, 0, 70);

	if (inStartButton)
		glColor3ub(255, 200, 200);
	else
		glColor3f(1, 1, 1);

	circle(0, 0, 67);

	glColor3ub(90, 100, 140);
	vprint2(-50, -12, 0.25, "START");

	glColor3ub(255, 255, 255);
	vprint(-190, -130, GLUT_BITMAP_HELVETICA_12, "Click Start BUtton");


	//glColor4f(1, 0, 0, 0.5);//transparency
	//circle(-70, 0, 30);

}
void displayRun()
{
	glColor3ub(90, 100, 140);
	vprint2(-50, 0, 0.35, "00:%02d", timeCounter);

	int width = (DURATION - timeCounter) * 30;

	glColor3ub(170, 250, 140);
	glRectf(-150, -70, -150 + width, -20);




}
void displayEnd()
{

	glColor3ub(90, 100, 140);
	vprint2(-50, 0, 0.35, "00:00");


	glColor3ub(255, 150, 140);
	glRectf(-150, -70, 150, -20);

	glColor3f(1, 1, 1);
	vprint(-190, -130, GLUT_BITMAP_HELVETICA_12, "Down Arrow To Open");

}
void displayOpen()
{
	glColor3ub(90, 100, 140);
	vprint2(-60, 30, 0.35, "DOOR");
	vprint2(-10, -20, 0.35, "IS");
	vprint2(-60, -70, 0.35, "OPEN");

	glColor3f(1, 1, 1);
	vprint(-190, -130, GLUT_BITMAP_HELVETICA_12, "UP Arrow to Close");

}

//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	diplayBackGround();

	switch (appstate)
	{
	case START:
		displayStart();
		break;
	case RUN:
		displayRun();
		break;
	case END:
		displayEnd();
		break;
	case OPEN:
		displayOpen();
		break;
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
	if (key == GLUT_KEY_DOWN && appstate == END)
		appstate = OPEN;


	if (key == GLUT_KEY_UP && appstate == OPEN)
		appstate = START;

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


	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		if (appstate == START && inStartButton)
		{
			appstate = RUN;
			timeCounter = DURATION;

			glutTimerFunc(TIMER_PERIOD, onTimer, 0); //invoke the timer

/*#if  TIMER_ON == 1
 // timer event
glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif*/

		}
	}

	if (button == GLUT_LEFT_BUTTON && stat GLUT_DOWN)
		printf("%d and %d ", px, py);

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

	int mx = x - winWidth / 2;
	int my = winHeight / 2 - y;


	inStartButton = checkCircle(mx, my, 0, 0, 70);


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {


	if (appstate == RUN)
	{
		timeCounter--;
		if (timeCounter > 0)
			glutTimerFunc(TIMER_PERIOD, onTimer, 0);
		else
			appstate = END;
	}

	// Write your codes here.





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
	glutCreateWindow("WOW MICREOWAVE");

	glutInitWindowPosition(200, 200);

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);



	Init();

	glutMainLoop();
}