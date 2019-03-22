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

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


#define MAX 20
typedef struct {
	double x, y;

}point_t;

typedef struct {
	point_t pos;
	double angle;
	double radius;
	bool hit;

}player_t;

typedef struct {
	point_t pos;
	double angle;
	unsigned char r, g, b;
	double radius;//ciricular collision boundery
	bool hit; // collison state

}entity_t;

entity_t icons[MAX];

int count = 0;

player_t p = { {0,0},45,20 };

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

void mycil(int x, int y, int r)
{
	//mycil(100, 100, 50);
	glBegin(GL_LINE_LOOP);
	for (float angle = 0; angle < 360; angle++)
	{
		glVertex2f(r*cos(angle*D2R), r*sin(angle*D2R));
	}
	glEnd();
}
void mysip(int x, int y, int r, float start, float end, float offset)
{
	//mysip(100,-100, 60, 0, 1008, 1);

	glBegin(GL_LINE_LOOP);
	for (float angle = start; angle < end; angle++)
	{
		r -= offset;
		glVertex2f(x + r*cos(angle*D2R), y + r*sin(angle*D2R));
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

//trnasformation
//need angle ratation
//move x and y need angle

void vertex(point_t p, point_t tr, double angle)
{
	double  xp = (p.x*cos(angle) - p.y*sin(angle)) + tr.x;
	double  yp = (p.x*sin(angle) + p.y*cos(angle)) + tr.y;

	glVertex2f(xp, yp);


}

void player(player_t p)
{
	double angle = p.angle*D2R;


	glColor3f(0.5, 0.5, 1);
	glBegin(GL_QUADS);
	vertex({ 8,25 }, p.pos, angle);
	vertex({ 8,-25 }, p.pos, angle);
	vertex({ -8,-20 }, p.pos, angle);
	vertex({ -8,20 }, p.pos, angle);
	glEnd();

	glColor3f(0.1, 0.1, 1);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	vertex({ 8,25 }, p.pos, angle);
	vertex({ 8,-25 }, p.pos, angle);
	vertex({ -8,-20 }, p.pos, angle);
	vertex({ -8,20 }, p.pos, angle);
	glEnd();


	glColor3f(0.8, 0.1, 0.1);
	circle(p.pos.x, p.pos.y, 15);

	glColor3f(0.1, 0.1, 1);
	circle_wire(p.pos.x, p.pos.y, 15);
	glLineWidth(1);

	glColor3f(1, 1, 0);
	vprint(p.pos.x - 7, p.pos.y - 5, GLUT_BITMAP_9_BY_15, "%0.0f", p.angle);

	if (p.hit)
	{
		glColor3f(0, 1, 0);
		circle_wire(p.pos.x, p.pos.y, p.radius);
	}
}

void drawicon(entity_t e)
{
	glColor3f(e.r, e.b, e.g);
	glBegin(GL_TRIANGLES);
	vertex({ 0,20 }, e.pos, e.angle);
	vertex({ -17.3,-10 }, e.pos, e.angle);
	vertex({ 17.3,-10 }, e.pos, e.angle);
	glEnd();



	/*glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	vertex({ 0,20 }, e.pos, e.angle);
	vertex({ 20,20 }, e.pos, e.angle);
	vertex({ 20,-20 }, e.pos, e.angle);
	vertex({ 20,0 }, e.pos, e.angle);
	glEnd();
	glLineWidth(1);*/


	glColor3f(e.r,e.b, e.g);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	vertex({ 0,20 }, e.pos, e.angle);
	vertex({ -17.3,-10 }, e.pos, e.angle);
	vertex({ 17.3,-10 }, e.pos, e.angle);
	glEnd();
	glLineWidth(1);

	if (e.hit)
	{
		glColor3f(0, 0, 1);
		circle_wire(e.pos.x, e.pos.y, e.radius);
	}

}

void drawallicon()
{
	for (int i = 0; i < count; i++)
		drawicon(icons[i]);
}

double dist(point_t p1, point_t p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

bool hitToIcons(player_t *p, entity_t icons[], int size)
{
	for (int i = 0; i < size; i++)
	{
		double d = dist(p->pos, icons[i].pos);
		if (d <= (p->radius + icons[i].radius))
		{
			icons[i].hit = true;
			p->hit = true;
		}
	}
	return p->hit;
}

void moveplayer(player_t*p, double speed)
{
	double dx = speed * cos(p->angle*D2R);
	double dy = speed * sin(p->angle*D2R);

	point_t oldPos = p->pos;
	p->pos = { p->pos.x + dx,p->pos.y + dy };
	//if playe hits move stop
	if (hitToIcons(p, icons, count))
		p->pos = oldPos;

	//chech window
	if (p->pos.x > (300 - p->radius) || p->pos.x < (-300 + p->radius) ||
		p->pos.y >(300 - p->radius) || p->pos.y < (-300 + p->radius))
	{
		p->pos = oldPos;

	}
}

void turnplayer(player_t *p, double turn)
{
	p->angle += turn;
	if (p->angle < 0)
		p->angle += 360;
	if (p->angle >= 360)
		p->angle += -360;

}
void resetcol()
{
	p.hit= false;
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


	player(p);
	drawallicon();



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

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && count < MAX)
	{
		icons[count].pos = { x - winWidth / 2.0,winHeight / 2.0 - y };
		icons[count].r = rand() % 256;
		icons[count].g = rand() % 256;
		icons[count].b = rand() % 256;
		icons[count].radius = 20;
		count++;
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

	resetcol();

	if (right)
	{
		turnplayer(&p, -4);
	}
	if (left)
	{
		turnplayer(&p, 4);
	}
	if (up)
	{
		moveplayer(&p, 5);
	}
	if (down)
	{
		moveplayer(&p, -5);
	}

	if (count > 0)
	{
		int near = 0;
		double minD = dist(p.pos, icons[0].pos);
		for (int i = 0; i < count; i++)
		{
			double d = dist(p.pos, icons[i].pos);
				if (d < minD)
				{
					minD = d;
					near = i;
				}
		}
		icons[near].angle += 2;
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
	glutCreateWindow("Template File");

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