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

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  300  // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

/*gLobal variables and constets for this prgram*/
//states of the prgram
#define LOAD 0
#define MENU 1
#define RUN	2
#define GAMEOVER 3
#define OPTION 4

#define BLOCK_NUMBERS	30
#define MIN_TIME_PERIOD 400
#define TIME_DECREMENT	50
#define SQUARE 10
#define CIRCLE 11



//load screen and genelral
int gameState = LOAD;
int dotCount = 0;
int elapsTime = 0; //load screen
int r = 180, g = 172, b = 50; //color
int r1 = 60, g2 = 30, b3 = 240; //color cell border
int  cellSate = SQUARE; //cell state
int WorngB; //wong clikc conter

//Runnin game variablebs
int cx, cy;//random selected cells cordinats
int mx, my;//current cell of mouese position ( CELL INDEX )
int score;//user cilk the pop cell
int counter;//how many cell are oped uped
bool active;//trick  stoper (user cannot pres mrore than once)
int timer;

void initGlobals()
{
	score = 0;
	counter = 0;
	timer = 300;
	//initally mae the current selcetion out side the window
	cx = cy = mx = my = -100;
	active = true;

}

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

void displayLoad()
{
	vprint2(-50, -50, 0.2, "LOADING");

	for (int i = 0; i < dotCount; i++)
		vprint2(80 + i * 20, -50, 0.2, ".");

}

void displayMenu()
{
	glColor3f(1, 1, 0);
	vprint(-30, 100, GLUT_BITMAP_TIMES_ROMAN_24, "MENU");

	glColor3f(1, 1, 1);
	vprint(-50, 50, GLUT_BITMAP_HELVETICA_18, "START GAME");
	vprint(-50, 20, GLUT_BITMAP_HELVETICA_18, "OPTION ( Press q )");
	vprint(-50, -10, GLUT_BITMAP_HELVETICA_18, "EXIT ( Escape )");

	glColor3f(0, 1, 0);
	vprint(-100, 230, GLUT_BITMAP_9_BY_15, "SpaceBar to Start Game");



}

void drawGrid()
{
	glColor3f(0.6, 0.6, 0.6);
	for (int i = -4; i <= 4; i++)
	{
		glBegin(GL_LINES);
		///vertical line
		glVertex2f(i * 50, -200);
		glVertex2f(i * 50, 200);

		///Horiaznal lines
		glVertex2f(-200, i * 50);
		glVertex2f(200, i * 50);


		glEnd();
	}

}

void borderCell()
{

	if (mx <= 3 && mx >= -4 && my <= 3 && my >= -4)
	{
		int posx = mx * 50;
		int posy = my * 50;
		glColor3ub(r, g, b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(posx, posy);
		glVertex2f(posx, posy + 50);
		glVertex2f(posx + 50, posy + 50);
		glVertex2f(posx + 50, posy);
		glEnd();
	}

}

void drawCell()
{
	if (cellSate == SQUARE)
	{
		int sx = cx * 50;
		int sy = cy * 50;
		glColor3ub(r1, g2, b3);
		glRectf(sx + 3, sy + 3, sx + 47, sy + 47);
		//glRectf(sx + 3, sy +4, sx + 47, sy + 47);
	}


	if (cellSate == CIRCLE)
	{

		int sx = cx * 50;
		int sy = cy * 50;
		glColor3ub(r, g, b);
		circle(sx + 25, sy + 25, 23);

	}

}

void displayRun()
{
	drawGrid();
	borderCell();
	drawCell();

	glColor3f(1, 1, 1);
	vprint(-240, -240, GLUT_BITMAP_9_BY_15, "Score : %d/%d ", score, counter);
	vprint(0, -240, GLUT_BITMAP_9_BY_15, "Worng Button Counter : %d ", WorngB);
}

void displayGameOver()
{
	glColor3f(1, 1, 0);
	vprint(-50, 0, GLUT_BITMAP_9_BY_15, "Socre : %d/%d", score, counter);
	vprint(-50, -30, GLUT_BITMAP_9_BY_15, "Worng Button Counter : %d ", WorngB);

	glColor3f(1, 0.5, 0.5);
	vprint(-70, 30, GLUT_BITMAP_9_BY_15, "GAME OVER");

	glColor3f(1, 0.5, 0.5);
	vprint(-70, -230, GLUT_BITMAP_8_BY_13, "- F5 to go to MENU -");
}

void displayOption()
{
	glColor3f(1, 1, 0);
	vprint(-50, 100, GLUT_BITMAP_TIMES_ROMAN_24, "OPTIONS");

	glColor3ub(r, g, b);
	vprint(-150, 50, GLUT_BITMAP_8_BY_13, "- F1 to Change Color of the Square -");
	glColor3ub(r1, g2, b3);
	vprint(-130, 36, GLUT_BITMAP_8_BY_13, "- and the cell borders - ");


	glColor3f(1, 1, 1);
	vprint(-150, 0, GLUT_BITMAP_8_BY_13, "- F2 to Change The Square TO Cricle -");
	if (gameState == OPTION && cellSate == SQUARE)
	{
		glColor3ub(r, g, b);
		glRectf(155, -5, 175, 15);

	}
	if (gameState == OPTION && cellSate == CIRCLE)
	{
		glColor3ub(r, g, b);
		circle(160, 5, 10);
	}

	glColor3f(1, 1, 1);
	vprint(-150, -36, GLUT_BITMAP_8_BY_13, "- F3 to Return to MENU -");


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


	switch (gameState)
	{
	case LOAD:
		displayLoad();
		break;
	case MENU:
		displayMenu();
		break;
	case OPTION:
		displayOption();
		break;
	case RUN:
		displayRun();
		break;
	case GAMEOVER:
		displayGameOver();
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

	if (key == 'q' && gameState == MENU)
		gameState = OPTION;
	else if (key == ' ' && gameState == MENU)
	{
		gameState = RUN;
		timer = 2000;
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
	if (key == GLUT_KEY_F5 && gameState == GAMEOVER)
	{
		initGlobals();
		gameState = MENU;
	}

	if (key == GLUT_KEY_F1 && gameState == OPTION)
	{
		//color
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

		r1 = rand() % 256;
		g2 = rand() % 256;
		b3 = rand() % 256;
	}

	if (key == GLUT_KEY_F2 && gameState == OPTION)
	{
		//cirlce to square
		if (cellSate == SQUARE)
			cellSate = CIRCLE;
		else if (cellSate == CIRCLE)
			cellSate = SQUARE;

	}
	if (key == GLUT_KEY_F3 && gameState == OPTION)
	{
		//Menu
		gameState = MENU;
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

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		if (mx == cx && my == cy && active && gameState == RUN)
		{
			score++;
			active = false;
		}

	if (gameState != RUN)
		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
			printf("Event : CLK %d %d\n", x - winHeight / 2, winHeight / 2 - y);

	if (gameState == RUN)
		if ((button == GLUT_RIGHT_BUTTON || button == GLUT_MIDDLE_BUTTON) && stat == GLUT_DOWN)
			WorngB++;


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
	// from glut to OpneGL

	x = x - winWidth / 2;
	y = winHeight / 2 - y;
	//from openGl to Block cordinate
	mx = floor(x / 50.0f);
	my = floor(y / 50.0f);



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(timer, onTimer, 0);
	// Write your codes here.

	if (gameState == LOAD)
	{
		dotCount = (dotCount + 1) % 4;
		elapsTime++;
		if (elapsTime == 8)
			gameState = MENU;
	}

	else if (gameState == RUN)
	{
		if (counter == BLOCK_NUMBERS)
			gameState = GAMEOVER;
		else
		{
			cx = rand() % 8 - 4;//%8 makes [0-7] so -4
			cy = rand() % 8 - 4;
			active = true;
			if (timer > MIN_TIME_PERIOD)
				timer -= TIME_DECREMENT;
			counter++;
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

	srand(time(NULL));
	initGlobals();


}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("WoW GAME");

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