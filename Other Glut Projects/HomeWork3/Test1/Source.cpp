/*********
CTIS164 - Template Source Program
----------
STUDENT : 
SECTION : 3
HOMEWORK: Homework #3
----------
Bonus:
1. Has load screen
2. Has Menu
3. Has Options to change the player
4. Has consequtive fire
5. Has two timers for load screen and for the game
---------
PROBLEMS:
Can Be hit but sometimes doesn't work Cant figure out why
No random Direction
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>


#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 750

#define  TIMER_PERIOD  16 // 60fps
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false;
int  winWidth, winHeight; // current Window width and height

						  /* User defined Types and Variables */
#define LOAD 0 //Game States
#define MENU 1
#define RUN 2
#define OPTION 3
#define END 4
int gamestate = LOAD;
#define Woplayer 30 //Player 1
#define Zaplayer 29 //Player 2
double deltaX;
double deltaY;
double rad;
int Rt[3], Gt[3], Bt[3];//Color test
void Init();
int playerstate = Zaplayer;//initial player


float A = 200, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift

typedef struct {
	float x, y;
} point_t;

typedef struct {
	float r, g, b;//for random color
} color_t;

typedef struct {
	int r[3];//orbits
	int rad[3] = { 10,15,13 };//for initilizing --make it random elsewhere
	color_t color[3];
	float angle[3] = { 0,0,0 };//start angle initilizing  --make it random elsewhere
	int test[3] = { 0, 0, 0 };
} ball_t;

typedef struct {
	point_t pos;    // position of the object
	float   angle;  // view angle 
	float   r;
} player_t;

typedef struct {
	point_t pos;
	float angle;
	bool active;//Hit or not hit
	float radius;
} fire_t;

typedef struct {
	point_t center;
	ball_t ball;
	bool hit = false;//Hit or not hit
} target_t;

int wide;
int counter = 1000;//for load screen
#define LENGTH 10
#define MAX_FIRE 3  // 3 consequtive fire (burst mode).
#define FIRE_RATE 4 // consecutive fire
int trac = 0; //for load screen
player_t p = { { 0,0 }, 45, 20 };//initilize player pos
fire_t   fr[MAX_FIRE];
int fire_rate = 0;
target_t target;
ball_t ball;
int timer = LENGTH;
int reset = 0;//tarhet hit counter
int shots = 0;//shots fired

void resetTarget(int i);//prptoytpe

void initGlobal()
{
	reset = 0;
	p = { { 0,0 }, 45, 20 };
	fr[MAX_FIRE];
	counter = 1000;
	for (int i = 0; i < 3; i++)
	{
		ball.rad[i] = 15;
		ball.angle[i] = 0;
		Gt[i] = 1;
		Bt[i] = 1;
		Gt[i] = 1;
		ball.test[i] = 0;
	}
	float XX = A + 50, YY = A + 100;
	ball.r[0] = fabs(A);
	ball.r[1] = fabs(XX);
	ball.r[2] = fabs(YY);
	target.hit = false;
	fr->active = true;
	shots = 0;
	reset = 0;


}

void vertex(point_t p, point_t tr, double angle)
{
	//for move able objets other than circles
	double  xp = (p.x*cos(angle* D2R) - p.y*sin(angle* D2R)) + tr.x;
	double  yp = (p.x*sin(angle* D2R) + p.y*cos(angle* D2R)) + tr.y;

	glVertex2f(xp, yp);

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

void drawaxsis() {

	glColor3f(0.6, 0.6, 0);
	glBegin(GL_LINES);
	// x axis
	glVertex2f(-300, 0);
	glVertex2f(300, 0);

	// y axis
	glVertex2f(0, -300);
	glVertex2f(0, 300);
	glEnd();

	glColor3f(0.3, 0.3, 1.0);
	for (int i = 200; i <= 300; i += 50)
	{
		circle_wire(0, 0, i);
	}
}

void drawPlayer(player_t tp) {

	if (playerstate == Woplayer) {
		glColor3f(0, 1, 1);//First triangle
		glBegin(GL_TRIANGLES);
		vertex({ -30,-20 }, tp.pos, tp.angle);
		vertex({ -30,20 }, tp.pos, tp.angle);
		vertex({ +40,0 }, tp.pos, tp.angle);
		glEnd();
		glColor3f(1, 1, 0);//second triangle
		glBegin(GL_TRIANGLES);
		vertex({ -25,-15 }, tp.pos, tp.angle);
		vertex({ -25,15 }, tp.pos, tp.angle);
		vertex({ +30,0 }, tp.pos, tp.angle);
		glEnd();
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex2f(tp.pos.x, tp.pos.y);
		glVertex2f(tp.pos.x + 55 * cos(tp.angle * D2R), tp.pos.y + 55 * sin(tp.angle * D2R));
		glEnd();
		//angle show
		glColor3f(1, 0, 0);
		vprint(tp.pos.x - 12, tp.pos.y - 5, GLUT_BITMAP_8_BY_13, "%.0f", tp.angle + 180);
		glLineWidth(1);
	}


	if (playerstate == Zaplayer) {
		glColor3f(0.5, 0.5, 1);//Fins
		glBegin(GL_QUADS);
		vertex({ 8,20 }, tp.pos, tp.angle);
		vertex({ 8,-20 }, tp.pos, tp.angle);
		vertex({ -8,-30 }, tp.pos, tp.angle);
		vertex({ -8,30 }, tp.pos, tp.angle);
		glEnd();

		glColor3f(1, 1, 0);//Cirles and lines
		glLineWidth(11);
		glBegin(GL_LINES);
		glVertex2f(tp.pos.x, tp.pos.y);
		glVertex2f(tp.pos.x + 55 * cos(tp.angle * D2R), tp.pos.y + 55 * sin(tp.angle * D2R));
		glEnd();
		glLineWidth(3);
		glColor3f(0.3, 0.3, 1.0);
		circle(tp.pos.x, tp.pos.y, tp.r);
		glColor3f(1, 0, 0);
		circle(tp.pos.x, tp.pos.y, tp.r - 4);

		//angle show
		glColor3f(1, 1, 1);
		vprint(tp.pos.x - 12, tp.pos.y - 5, GLUT_BITMAP_8_BY_13, "%.0f", tp.angle + 180);
		glLineWidth(1);
	}


	/*glColor3f(e.r, e.b, e.g);
	glBegin(GL_TRIANGLES);
	vertex({ 0,20 }, e.pos, e.angle);
	vertex({ -17.3,-10 }, e.pos, e.angle);
	vertex({ 17.3,-10 }, e.pos, e.angle);
	glEnd();*/

	//forward pointer
	float face = A - 50;
	float faceRad = fabs(face);
	glColor3f(1, 1, 0);
	circle(faceRad * cos(tp.angle * D2R), faceRad * sin(tp.angle * D2R), 5);


	glColor3f(1, 1, 1);
}

void drawFires()
{
		for (int i = 0; i < MAX_FIRE; i++)
		{
			if (fr[i].active)
			{
				glColor3f(1, 1, 1);
				circle(fr[i].pos.x, fr[i].pos.y, 6);
			}
		}
		
	glLineWidth(1);
}

float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
	//for orbits
}

void color()
{
	//color random on next program
	for (int i = 0; i < 3; i++)
	{
		ball.color[i].r = rand() % 256 + 1;
		ball.color[i].b = rand() % 256 + 1;
		ball.color[i].g = rand() % 256 + 1;
		ball.rad[i] = rand() % 20 + 10;
	}
}

void balls()
{
	float XX = A + 50, YY = A + 100;

	ball.r[0] = fabs(A);//The orbits
	ball.r[1] = fabs(XX);
	ball.r[2] = fabs(YY);

	for (int wow = 0; wow < 3; wow++)//testing color
	{
		Rt[wow] = 1;
		Bt[wow] = 1;
		Gt[wow] = 1;
	}

	for (int i = 0; i < 3; i++)
	{
		if (!target.hit)//hit cheack
		{
			if (ball.test[i] == 0)
			{
				glColor3ub(ball.color[i].r, ball.color[i].g, ball.color[i].b);
				circle(ball.r[i] * cos(ball.angle[i] * D2R), ball.r[i] * sin(ball.angle[i] * D2R), ball.rad[i]);

				glColor3f(Rt[i], Gt[i], Bt[i]);
				vprint(ball.r[i] * cos(ball.angle[i] * D2R), ball.r[i] * sin(ball.angle[i] * D2R), GLUT_BITMAP_8_BY_13, "%.0f", fabs(ball.angle[i]));
			}
		}
	}
}

void resetTarget(int i)
{
	//reset targets in a wierd way
	ball.test[i] = 1;
	Rt[i] = 0, Gt[i] = 0, Bt[i] = 0;
	ball.color[i].r = rand() % 256 + 1;
	ball.color[i].g = rand() % 256 + 1;
	ball.color[i].b = rand() % 256 + 1;
	reset++;
}

double dist(point_t p1, point_t p2)
{
	//distance function fo two points
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

void testCollision(fire_t *fr, target_t t) {

	//test if hit or not
	for (int i = 0; i < 3; i++)
	{
		t.center.x = ball.r[i] * cos(ball.angle[i] * D2R);
		t.center.y = ball.r[i] * sin(ball.angle[i] * D2R);
		double d = dist(fr->pos, t.center);
		if (d <= (fr->radius + t.ball.rad[i] + 2))
		{
			t.hit = true;
			fr->active = false;
			resetTarget(i);
		}
	}

}

void displayOption()
{
	//show your options
	//show diffrest bulletS
	drawPlayer(p);

	glColor3f(1, 1, 0);
	vprint(-50, 200, GLUT_BITMAP_TIMES_ROMAN_24, "OPTIONS");
	glColor3f(1, 1, 1);
	vprint(-100, 150, GLUT_BITMAP_8_BY_13, "- F2 to Change Player -");
	vprint(-100, 110, GLUT_BITMAP_8_BY_13, "- F3 to Return to MENU -");

	glLineWidth(1);
}

void gameEND()
{
	//end game screen
	vprint(-100, 50, GLUT_BITMAP_TIMES_ROMAN_24, "- Click to resart -");

}

void display()
{

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);



	if (gamestate == LOAD)//load state
	{
		drawLoad();
		glColor3f(0, 1, 0);
		glRectf(-184, -95, 135, -130);
		glColor3f(0.8, 0.8, 0.8);
		glRectf(-180, -100, -140 + wide, -125);
	}
	else if (gamestate == MENU)//menu state
	{
		glColor3f(1, 1, 0);
		vprint(-190, 100, GLUT_BITMAP_TIMES_ROMAN_24, "Hit The Moving Objet Simulator");
		glColor3f(1, 1, 1);
		vprint(-150, 50, GLUT_BITMAP_HELVETICA_18, "START GAME ( Press Enter )");
		vprint(-150, 20, GLUT_BITMAP_HELVETICA_18, "OPTION ( Press Q )");
		vprint(-150, -10, GLUT_BITMAP_HELVETICA_18, "EXIT ( Escape )");
		glColor3f(0.5, 1, 0);
		vprint(-240, -50, GLUT_BITMAP_HELVETICA_18, "--Can use Left Mouse button and Spacebar to fire --");
		color();

	}
	else if (gamestate == OPTION)//show options
	{
		displayOption();
	}
	else if (gamestate == RUN)//run the game
	{
		drawaxsis();
		drawFires();
		drawPlayer(p);
		balls();

		glRectf(250, -250, 390, -350);
		glColor3f(0.5, 0, 0.5);
		glRectf(270, -270, 370, -330);
		glColor3f(1, 1, 1);
		vprint(290, -300, GLUT_BITMAP_HELVETICA_18, "%d", reset / 3);
		vprint(280, -320, GLUT_BITMAP_HELVETICA_18, "Hits");

		vprint(340, -300, GLUT_BITMAP_HELVETICA_18, "%d", shots - 1);
		vprint(320, -320, GLUT_BITMAP_HELVETICA_18, "Shots");
		if (reset / 2 == 3)
		{
			gamestate = END;
			reset = 0;
		}
	}
	else if (gamestate == END)//game end
	{
		int wow;
		wow = reset;

		glRectf(250, -250, 390, -350);
		glColor3f(0.5, 0, 0.5);
		glRectf(270, -270, 370, -330);
		glColor3f(1, 1, 1);
		vprint(290, -300, GLUT_BITMAP_HELVETICA_18, "%d", reset);
		vprint(280, -320, GLUT_BITMAP_HELVETICA_18, "Hits");

		vprint(340, -300, GLUT_BITMAP_HELVETICA_18, "%d", shots - 1);
		vprint(320, -320, GLUT_BITMAP_HELVETICA_18, "Shots");
		gameEND();
		drawaxsis();
		//drawFires();
		drawPlayer(p);

	}

	glutSwapBuffers();
}

// In the fire array, check if any fire is available.
// if there is a fire that we can use for firing, return its index.
int findAvailableFire()
{

	for (int i = 0; i < MAX_FIRE; i++)
	{
		if (fr[i].active == false) return i;
	}
	return -1;
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		spacebar = true;
	//key = 'enter' and go to menu
	if (key == 13 && gamestate == MENU)
		gamestate = RUN;
	//got to option
	if (key == 'q' && gamestate == MENU)
		gamestate = OPTION;
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		spacebar = false;
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
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
	//glutPostRedisplay() ;
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	if (key == GLUT_KEY_F3 && gamestate == OPTION)
	{
		//Menu
		gamestate = MENU;
	}
	if (key == GLUT_KEY_F2 && gamestate == OPTION)
	{
		//player option
		if (playerstate == Woplayer)
			playerstate = Zaplayer;
		else if (playerstate == Zaplayer)
			playerstate = Woplayer;
	}



	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
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
		spacebar = true;
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_UP)
		spacebar = false;

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		printf("Event : CLK %d %d\n", x - winWidth / 2, winHeight / 2 - y);

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && gamestate == END)
	{
		gamestate = MENU;
		Init();
	}

	// to refresh the window it calls display() function
	//glutPostRedisplay() ; 
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

	//its on onMove and onMoveDown because it makes it more realistic

	//move only when the game isnt finished
	if (gamestate != END) {
		deltaX = x - winWidth / 2 - 0;
		deltaY = winHeight / 2 - y - 0;
		rad = atan2(deltaY, deltaX);

		p.angle = rad * (180 / PI);
	}
	// to refresh the window it calls display() function   
	//glutPostRedisplay() ;
}

void onMove(int x, int y) {

	//move only when the game isnt finished
	if (gamestate != END) {
		deltaX = x - winWidth / 2 - 0;
		deltaY = winHeight / 2 - y - 0;
		rad = atan2(deltaY, deltaX);

		p.angle = rad * (180 / PI);
	}
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
}

//firts times for normal stuff on game
#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here
	srand(time(NULL));

	//random speed 
	ball.angle[0] += rand() % 1 + 1.5;
	if (ball.angle[0] > 360)
		ball.angle[0] -= 360;

	ball.angle[1] += rand() % 1 + 1;
	if (ball.angle[1] > 360)
		ball.angle[1] -= 360;

	ball.angle[2] -= rand() % 1 + 1.5;
	if (ball.angle[2] < -360)
		ball.angle[2] += 360;

	if (gamestate == RUN )
		if (spacebar && fire_rate == 0) {
			int availFire = findAvailableFire();
			if (availFire != -1) {
				fr[availFire].pos = p.pos;
				fr[availFire].angle = p.angle;
				fr[availFire].active = true;
				fire_rate = FIRE_RATE;
			}
		}

	if (fire_rate > 0)
		fire_rate--;

	// Move bullets that are active.
	for (int i = 0; i < MAX_FIRE; i++)
	{
		if (fr[i].active)
		{
			fr[i].pos.x += 10 * cos(fr[i].angle * D2R);
			fr[i].pos.y += 10 * sin(fr[i].angle * D2R);

			if (fr[i].pos.x > 400 || fr[i].pos.x < -400 || fr[i].pos.y > 400 || fr[i].pos.y < -400)
			{
				fr[i].active = false;
				shots++;
			}

			testCollision(fr, target);

		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif
/*second timer fo load screen because i was too lazy to make the necessary caculations with the first timer*/
void onTimer2(int v) {

	glutTimerFunc(200, onTimer2, 0);
	if (gamestate == LOAD)
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
	glutPostRedisplay();
}

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initGlobal();

}


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Levent Durdali HOMEWORK#3 -- Normal Hit The Ball Game --");

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
	glutTimerFunc(100, onTimer2, 0);
#endif

	Init();

	glutMainLoop();
}