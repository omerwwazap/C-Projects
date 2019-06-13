/*********
CTIS164 - Template Source Program
----------
STUDENT : Omer Levent Durdali
SECTION : 3
HOMEWORK: Homework #1
----------
PROBLEMS:
*********/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000 //like the given example
#define WINDOW_HEIGHT 600
#define LENGTH 10
#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define LOAD 0 //Game States
#define MENU 1
#define RUN 2
#define END 3
#define SHOWWIN 4
#define OPTION 5
#define WoRocket 66 //Rocket 1
#define ZaRocket 67 //Rocket 1

int ship = WoRocket; //initial rocket state
int gamestate = LOAD; //initial game state
int msecs, secs; //variable for timers 
int my1 = 0, py1 = 0, my2 = 0, py2 = 0, my3 = 0, py3 = 0,//Rocket Codinates for positive and negative 
my4 = 0, py4 = 0, my5 = 0, py5 = 0, my6 = 0, py6 = 0;//Rocket Codinates for positive and negative 
bool activeTimer = false;
int counter = TIMER_PERIOD;
int trac = 0; //for load screen
int timer = LENGTH;
int wide; //load screen bar
int winner; //winner variable
int max = 0; //variables for winner func
int min = 0; //variables for winner func
double first1 = 0, first2 = 0, first3 = 0, first4 = 0, first5 = 0, first6 = 0; //alpha color vailabes for fire
double last1 = 1, last2 = 1, last3 = 1, last4 = 1, last5 = 1, last6 = 1; //alpha color vailabes for fire
int RoHight = 0; // altitude of the rocket
#define D2R 0.0174532
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //
void initGlobals()
{
	gamestate = 0;
	msecs = secs = 0;
	my1 = py1 = my2 = py2 = my3 = py3 = my4 = py4 = my5 = py5 = my6 = py6 = 0;
	bool activeTimer = false;
	int counter = 0;
	trac = 0;
	timer = LENGTH;
	int ship = WoRocket;
	wide = 0;
	winner = 1;
	max = 0;
	min = 0;
	first1 = first2 = first3 = first4 = first5 = first6 = 0;
	last1 = last2 = last3 = last4 = last5 = last6 = 1;
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

//
// To display onto window using OpenGL commands
//
void drawLoad()
{
	int i;
	glColor3f(0, 1, 0);
	vprint(-470, 280, GLUT_BITMAP_9_BY_15, "Omer Levent Durdali");
	vprint(-420, 260, GLUT_BITMAP_9_BY_15, "21702600");
	glColor3f(1, 1, 1);
	vprint2(-150, 0, 0.5, "LOADING");
	vprint(-70, -30, GLUT_BITMAP_9_BY_15, "Please Wait");
	for (i = 0; i < trac; i++)
		vprint2(125 + i * 20, 0, 0.2, ".");
}

void drawBackground()
{
	glLineWidth(2);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(500, 200);
	glVertex2f(-500, 200);
	glEnd();

	glColor3f(1, 1, 1);
	vprint(-125, 285, GLUT_BITMAP_9_BY_15, "Rocket Landing by Levent Durdali");
	vprint(-160, 220, GLUT_BITMAP_9_BY_15, "Press SpacerBar to stop/continue the race");
	vprint(-160, 255, GLUT_BITMAP_9_BY_15, "Press F1 to restart the race when it ends");

	//Sun
	glColor3ub(255, 200, 56);
	circle(-420, 250, 50);
	vprint(-370, 215, GLUT_BITMAP_HELVETICA_18, "Sun");
	//timer on Sun
	glColor3ub(102, 0, 0);
	vprint(-448, 270, GLUT_BITMAP_HELVETICA_18, "TIMER");
	vprint2(-457, 240, 0.15, "00:%02d:%02d", secs, msecs);

	//Moon
	glColor3ub(29, 34, 57);
	circle(420, 250, 50);
	glColor3f(1, 1, 1);
	//winner on Moon
	vprint(390, 270, GLUT_BITMAP_HELVETICA_18, "Winner");
	vprint(410, 230, GLUT_BITMAP_TIMES_ROMAN_24, "0%d", winner);
	vprint(320, 220, GLUT_BITMAP_HELVETICA_18, "Moon");

	//Red borders for spaceship lians
	glLineWidth(2);
	glColor3f(1, 0, 0);
	//Choose not to do it with a forloop
	glBegin(GL_LINES);
	glVertex2f(-350, 200);
	glVertex2f(-350, -300);
	glVertex2f(-200, 200);
	glVertex2f(-200, -300);
	glVertex2f(-50, 200);
	glVertex2f(-50, -300);
	glVertex2f(100, 200);
	glVertex2f(100, -300);
	glVertex2f(250, 200);
	glVertex2f(250, -300);
	glVertex2f(400, 200);
	glVertex2f(400, -300);
	glEnd();
}

void winnerLead()
{
	//Show lead rokect
	switch (winner)
	{
	case 1:
		vprint(410, -235 + my1 - py1, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my1 - py1, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	case 2:
		vprint(410, -235 + my2 - py2, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my2 - py2, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	case 3:
		vprint(410, -235 + my3 - py3, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my3 - py3, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	case 4:
		vprint(410, -235 + my4 - py4, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my4 - py4, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	case 5:
		vprint(410, -235 + my5 - py5, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my5 - py5, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	case 6:
		vprint(410, -235 + my6 - py6, GLUT_BITMAP_TIMES_ROMAN_24, "<-%dft", RoHight);
		vprint(410, -260 + my6 - py6, GLUT_BITMAP_TIMES_ROMAN_24, "%d.Rocket", winner);
		break;
	}
	//go to Winner promt message
	if (RoHight == 0 && gamestate == END)
		gamestate = SHOWWIN;
}

void drawRocket(int y, int x, int num, int negatif, double first, double last)
{
	if (ship == WoRocket)
	{
		//fins
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2f(-450 + x, -230 + y - negatif);
		glVertex2f(-430 + x, -230 + y - negatif);
		glVertex2f(-420 + x, -255 + y - negatif);
		glVertex2f(-460 + x, -255 + y - negatif);
		glEnd();

		//top
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex2f(-440 + x, -200 + y - negatif);
		glVertex2f(-430 + x, -220 + y - negatif);
		glVertex2f(-450 + x, -220 + y - negatif);
		glEnd();

		//body
		glColor3f(1, 1, 1);
		glRectf(-450 + x, -220 + y - negatif, -430 + x, -280 + y - negatif);

		//window
		glColor3f(0.5, 0.5, 0.5);
		circle(-440 + x, -230 + y - negatif, 5);
		circle(-440 + x, -250 + y - negatif, 5);
		circle(-440 + x, -270 + y - negatif, 5);

		//land
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex2f(-440 + x, -280 + y - negatif);
		glVertex2f(-427 + x, -300 + y - negatif);
		glVertex2f(-430 + x, -280 + y - negatif);
		glEnd();
		glBegin(GL_TRIANGLES);
		glVertex2f(-440 + x, -280 + y - negatif);
		glVertex2f(-453 + x, -300 + y - negatif);
		glVertex2f(-450 + x, -280 + y - negatif);
		glEnd();

		//fire
		glColor4f(1, 0.5, 0.5, first);
		glBegin(GL_TRIANGLES);
		glVertex2f(-450 + x, -280 + y - negatif);
		glVertex2f(-430 + x, -280 + y - negatif);
		glVertex2f(-447 + x, -305 + y - negatif);
		glEnd();
		glColor4f(1, 0.5, 0.5, last);
		glBegin(GL_TRIANGLES);
		glVertex2f(-450 + x, -280 + y - negatif);
		glVertex2f(-430 + x, -280 + y - negatif);
		glVertex2f(-440 + x, -310 + y - negatif);
		glEnd();
		glColor4f(1, 0.5, 0.5, first);
		glBegin(GL_TRIANGLES);
		glVertex2f(-450 + x, -280 + y - negatif);
		glVertex2f(-430 + x, -280 + y - negatif);
		glVertex2f(-433 + x, -305 + y - negatif);
		glEnd();

	}
	if (ship == ZaRocket)
	{
		//top
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex2f(-440 + x, -200 + y - negatif);
		glVertex2f(-420 + x, -220 + y - negatif);
		glVertex2f(-460 + x, -220 + y - negatif);
		glEnd();

		//body
		glColor3f(1, 1, 1);
		glRectf(-461 + x, -220 + y - negatif, -421 + x, -280 + y - negatif);

		//window
		glColor3f(0.5, 0.5, 0.5);
		circle(-440 + x, -230 + y - negatif, 5);
		circle(-440 + x, -250 + y - negatif, 5);
		circle(-440 + x, -270 + y - negatif, 5);

		//land
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex2f(-461 + x, -280 + y - negatif);
		glVertex2f(-465 + x, -300 + y - negatif);
		glVertex2f(-450 + x, -280 + y - negatif);
		glEnd();
		glBegin(GL_TRIANGLES);
		glVertex2f(-421 + x, -280 + y - negatif);
		glVertex2f(-417 + x, -300 + y - negatif);
		glVertex2f(-433 + x, -280 + y - negatif);
		glEnd();

		//fire
		glColor4f(1, 0.5, 0.5, first);
		glBegin(GL_TRIANGLES);
		glVertex2f(-461 + x, -280 + y - negatif);
		glVertex2f(-421 + x, -280 + y - negatif);
		glVertex2f(-455 + x, -305 + y - negatif);
		glEnd();
		glColor4f(1, 0.5, 0.5, last);
		glBegin(GL_TRIANGLES);
		glVertex2f(-461 + x, -280 + y - negatif);
		glVertex2f(-421 + x, -280 + y - negatif);
		glVertex2f(-442 + x, -310 + y - negatif);
		glEnd();
		glColor4f(1, 0.5, 0.5, first);
		glBegin(GL_TRIANGLES);
		glVertex2f(-461 + x, -280 + y - negatif);
		glVertex2f(-421 + x, -280 + y - negatif);
		glVertex2f(-425 + x, -305 + y - negatif);
		glEnd();
	}

	//number of rocket
	glColor3ub(255, 215, 0);
	circle(-390 + x, -230 + y - negatif, 10);
	glColor3ub(255, 20, 147);
	vprint2(-393 + x, -235 + y - negatif, 0.1, "%d", num);
}

void displayOption()
{	//show your option
	drawRocket(my1 + 280, 100, 1, py1, 0, 0);
	glColor3f(1, 1, 0);
	vprint(-50, 100, GLUT_BITMAP_TIMES_ROMAN_24, "OPTIONS");
	glColor3f(1, 1, 1);
	vprint(-100, 50, GLUT_BITMAP_8_BY_13, "- F2 to Change Rocktes -");
	vprint(-100, 10, GLUT_BITMAP_8_BY_13, "- F3 to Return to MENU -");
}

void displayWin()
{	//dilsplay promt at the end of the race
	glColor3f(0, 0, 0);
	glRectf(-230, 155, 190, 10);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-230, 155);
	glVertex2f(190, 155);
	glVertex2f(190, 10);
	glVertex2f(-230, 10);
	glEnd();
	glColor3f(1, 1, 1);
	vprint2(-210, 80, 0.2, "The winner is the %d. Rocket", winner);
	vprint(-140, 40, GLUT_BITMAP_9_BY_15, "Press F1 to restart the race");
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (gamestate)
	{
	case LOAD:
		drawLoad();
		glColor3f(0, 1, 0);
		glRectf(-184, -95, 135, -130);
		glColor3f(0.8, 0.8, 0.8);
		glRectf(-180, -100, -140 + wide, -125);
		break;
	case MENU:
		glColor3f(1, 1, 0);
		vprint(-190, 100, GLUT_BITMAP_TIMES_ROMAN_24, "Rocket Landing Simlulator and a Race");
		glColor3f(1, 1, 1);
		vprint(-150, 50, GLUT_BITMAP_HELVETICA_18, "START GAME ( Press Enter )");
		vprint(-150, 20, GLUT_BITMAP_HELVETICA_18, "OPTION ( Press Q )");
		vprint(-150, -10, GLUT_BITMAP_HELVETICA_18, "EXIT ( Escape )");
		break;
	case OPTION:
		displayOption();
		break;
	case RUN:
		drawBackground();
		winnerLead();
		//(		   y,  x,num,negatif,rocket1,roket2)
		drawRocket(my1, 0, 1, py1, first1, last1);
		drawRocket(my2, 150, 2, py2, first2, last2);
		drawRocket(my3, 300, 3, py3, first3, last3);
		drawRocket(my4, 450, 4, py4, first4, last4);
		drawRocket(my5, 600, 5, py5, first5, last5);
		drawRocket(my6, 750, 6, py6, first6, last6);
		break;
	case END:
		drawBackground();
		RoHight = RoHight - RoHight;
		winnerLead();
		//(y,x,num,negatif,rocket1,roket2)
		drawRocket(my1, 0, 1, py1, first1, last1);
		drawRocket(my2, 150, 2, py2, first2, last2);
		drawRocket(my3, 300, 3, py3, first3, last3);
		drawRocket(my4, 450, 4, py4, first4, last4);
		drawRocket(my5, 600, 5, py5, first5, last5);
		drawRocket(my6, 750, 6, py6, first6, last6);
		break;
	case SHOWWIN:
		drawBackground();
		displayWin();
		winnerLead();
		drawRocket(my1, 0, 1, py1, first1, last1);
		drawRocket(my2, 150, 2, py2, first2, last2);
		drawRocket(my3, 300, 3, py3, first3, last3);
		drawRocket(my4, 450, 4, py4, first4, last4);
		drawRocket(my5, 600, 5, py5, first5, last5);
		drawRocket(my6, 750, 6, py6, first6, last6);
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
	//stop/resume game
	if (key == ' ' && gamestate == RUN)
		activeTimer = !activeTimer;
	//key = 'enter' and go to menu
	if (key == 13 && gamestate == MENU)
		gamestate = RUN;
	//got to option
	if (key == 'q' && gamestate == MENU)
		gamestate = OPTION;

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

	if (key == GLUT_KEY_F1 && gamestate == SHOWWIN || gamestate == END)
	{
		//restart game
		initGlobals();
		gamestate = MENU;
	}
	if (key == GLUT_KEY_F3 && gamestate == OPTION)
	{
		//Menu
		gamestate = MENU;
	}
	if (key == GLUT_KEY_F2 && gamestate == OPTION)
	{
		//Slim rocket to fat rocket
		if (ship == WoRocket)
			ship = ZaRocket;
		else if (ship == ZaRocket)
			ship = WoRocket;
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
	//for easier use of  the cordinates
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		printf("Event : CLK %d %d\n", x - winWidth / 2, winHeight / 2 - y);
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
	int x1 = x - winWidth / 2;
	int	y1 = winHeight / 2 - y;
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	//Random generate
	srand(time(NULL));
	glutTimerFunc(counter, onTimer, 0);

	//Loadstate bar lenght
	if (gamestate == LOAD && activeTimer == false)
	{
		trac = (trac + 1) % 4;
		timer--;
		//Bar lenght
		wide = (LENGTH - timer) * 30;
		if (timer == 0)
		{
			//Go to Menu
			counter = 50;
			gamestate = MENU;
		}
	}

	if (gamestate == RUN && activeTimer == true)
	{
		//initlize maximum and minimum values for winner
		//my* to increase altitude
		//py* to decrease altitude
		/*they are diffrent beacuse its easier to ramdomize increas and decrease it like this plus it makes the race more divers.*/
		min = max = my1 + py1;
		//initlizinf the winner
		winner = 1;
		//miliseconds for timer
		msecs = msecs + 5;
		//milisecons to seconds for timer
		if (msecs == 100)
		{
			msecs = 0;
			secs = secs + 1;
			if (-200 + my1 <= 200)
				RoHight += 1500;
			else if (-200 + my1 - py1 > -205)
			{
				RoHight = RoHight - 1500;
				if (RoHight <= 500)
					RoHight = 0;
			}
		}
		//1. random and hight check
		if (-200 + my1 <= 200)
		{
			my1 = my1 + rand() % 3 + 3;
			last1 = last1 - 0.01;
		}
		else if (-200 + my1 - py1 > -205)
		{
			py1 = py1 + rand() % 3 + 2;
			first1 = first1 + 0.01;
		}
		else
			gamestate = END;
		//2. random and hight check
		if (-200 + my2 <= 200)
		{
			my2 = my2 + rand() % 3 + 3;
			last2 = last2 - 0.01;
		}
		else if (-200 + my2 - py2 > -205)
		{
			py2 = py2 + rand() % 3 + 2;
			first2 = first2 + 0.01;
		}
		else
			gamestate = END;
		//3. random and hight check
		if (-200 + my3 <= 200)
		{
			my3 = my3 + rand() % 3 + 3;
			last3 = last3 - 0.01;
		}
		else if (-200 + my3 - py3 > -205)
		{
			py3 = py3 + rand() % 3 + 2;
			first3 = first3 + 0.01;
		}
		else
			gamestate = END;
		//4. random and hight check
		if (-200 + my4 <= 200)
		{
			my4 = my4 + rand() % 3 + 3;
			last4 = last4 - 0.01;
		}
		else if (-200 + my4 - py4 > -205)
		{
			py4 = py4 + rand() % 3 + 2;
			first4 = first4 + 0.01;
		}
		else
			gamestate = END;
		//5. random and hight check
		if (-200 + my5 <= 200)
		{
			my5 = my5 + rand() % 3 + 2;
			last5 = last5 - 0.01;
		}
		else if (-200 + my5 - py5 > -205)
		{
			py5 = py5 + rand() % 3 + 2;
			first5 = first5 + 0.01;
		}
		else
			gamestate = END;
		//6. random and hight check
		if (-200 + my6 <= 200)
		{
			my6 = my6 + rand() % 3 + 3;
			last6 = last6 - 0.01;
		}
		else if (-200 + my6 - py6 > -205)
		{
			py6 = py6 + rand() % 3 + 2;
			first6 = first6 + 0.01;
		}
		else
			gamestate = END;

		/*winner finder!!!
		Really long but works!
		My original one didn't work very well,
		so i googled and found one for a 3 way winner function and modified it to work as a 6 way function*/
		if (my2 + py2 > max)
		{
			max = my2 + py2;
			winner = 2;
		}
		else if (my2 + py2 < min)
		{
			min = my2 + py2;
		}
		if (my3 + py3 > max)
		{
			max = my3 + py3;
			winner = 3;
		}
		else if (my3 + py3 < min)
		{
			min = my3 + py3;
		}
		if (my4 + py4 > max)
		{
			max = my4 + py4;
			winner = 4;
		}
		else if (my4 + py4 < min)
		{
			min = my4 + py4;
		}
		if (my5 + py5 > max)
		{
			max = my5 + py5;
			winner = 5;
		}
		else if (my5 + py5 < min)
		{
			min = my5 + py5;
		}
		if (my6 + py6 > max)
		{
			max = my6 + py6;
			winner = 6;
		}
		else if (my6 + py6 < min)
		{
			min = my6 + py6;
		}
		//yes its really long
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	srand(time(NULL));
	initGlobals();
}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("- SpaceX Landing Slimulator but as a Race -");

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
	glutTimerFunc(counter, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}