

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD   16 //period for the timer
#define TIMER_ON        1 //0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

float A = 100, //amplitude
      fq = 1,  //frequency
      C = 0,   //horizontal phase shift
      B = 0;   //vertical phase shift
float angle = 0;

typedef struct {
   float x, y;
} pos_t;

pos_t p0, //red point: vertical harmonic motion
      p1, //green point: horizontal harmonic motion
      p2; //composite animation for color, radius and position 

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

/*****
   vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
*****/
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

void drawFunction() {
   glColor3f(1, 1, 1);
   vprint(-130, 270, GLUT_BITMAP_TIMES_ROMAN_24, "HARMONIC MOTIONS");
   vprint(-200, 210, GLUT_BITMAP_TIMES_ROMAN_24,
      "f(x) = %.0fsin(%.0fx + %.0f) + %.0f", A, fq, C, B);
   glColor3f(1, 1, 0);
   vprint(-220, 170, GLUT_BITMAP_8_BY_13,
      "Amplitude: %.0f  Freq: %.0f  Period: %.0f  Phase: %.0f  Ver-Shift: %.0f",
      fabs(A), fabs(fq), 360.0 / fabs(fq), C, B);
   vprint(-350, -290, GLUT_BITMAP_8_BY_13,
       "Press [F1/F2:Amplitude] [F3/F4:Frequency] [F5/F6:Phase] [F7/F8:Vertical Shift] to modify");
}

float f(float x) {
   return A * sin((fq * x + C) * D2R) + B;
}

void drawGraph() {
   glColor3f(0.6, 0.6, 0.6);
   glBegin(GL_LINES);
   glVertex2f(-350, 150);
   glVertex2f(-350, -100);

   glVertex2f(-380, 25);
   glVertex2f(380, 25);
   glEnd();
   glColor3f(1, 1, 0.5);
   glBegin(GL_LINE_STRIP);
   for (float x = 0; x <= 360; x += 2) {
      glVertex2f(x - 350, f(x) + 25);
   }
   glEnd();
}

void drawAngle() {
   float r = fabs(A); //amplitude

   glColor3f(0.3, 0.3, 1.0);
   circle_wire(200, 25, r); //blue circle on the right

   glColor3f(1, 1, 1);
   vprint(190, 30, GLUT_BITMAP_8_BY_13, "%0.f", angle); //white angle

   glColor3f(1, 0, 0);
   //red point on blue circle
   circle(r*cos(angle * D2R) + 200, r*sin(angle*D2R) + 25, 5);
   //red point on sinusoidal
   circle(angle - 350, f(angle) + 25, 5);
}

/*****
   to display onto window using OpenGL commands
*****/
void display() {
   //clear window to black
   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   drawFunction();
   drawGraph();
   drawAngle();

   //Vertical Harmonic Motion
   glColor3f(1, 0, 0);
   circle(p0.x, p0.y, 20);
   glColor3f(1, 1, 1);
   vprint(p0.x - 13, p0.y - 5, GLUT_BITMAP_HELVETICA_10, "%.1f", p0.y - 25);
   //since 25 was added to position on the coordinate system, it is subtracted

   //Horizontal Harmonic Motion
   glColor3f(0, 1, 0);
   circle(p1.x, p1.y, 20);
   glColor3f(0, 0, 0);
   vprint(p1.x - 13, p1.y - 5, GLUT_BITMAP_HELVETICA_10, "%.1f", p1.x);

   //Composite animation for color, radius and position.
   float c = 0.5 * sin(0.5 * angle * D2R) + 0.5; //between c=0 and c=1.0
   glColor3f(c, c*c, 1-c);
   circle(p2.x, p2.y, 30 * sin(3*angle*D2R) + 40);

   glutSwapBuffers();
}

/*****
   key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
*****/
void onKeyDown(unsigned char key, int x, int y) {
   //exit when ESC is pressed.
   if (key == 27)
      exit(0);

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

   switch (key) {
   case GLUT_KEY_UP:    up = true; break;
   case GLUT_KEY_DOWN:  down = true; break;
   case GLUT_KEY_LEFT:  left = true; break;
   case GLUT_KEY_RIGHT: right = true; break;
   case GLUT_KEY_F1:    A += 10; break;
   case GLUT_KEY_F2:    A -= 10; break;
   case GLUT_KEY_F3:    fq++; break;
   case GLUT_KEY_F4:    fq--; break;
   case GLUT_KEY_F5:    C += 30; break;
   case GLUT_KEY_F6:    C -= 30; break;
   case GLUT_KEY_F7:    B += 10; break;
   case GLUT_KEY_F8:    B -= 10; break;
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

   angle += 2; 
   if (angle > 360)
      angle -= 360;
    
   //red point: Vertical Motion
   p0.x = -350;
   p0.y = f(angle);
   p0.y += 25; //25 is added to position on the coordinate system

   //green point: Horizontal Motion
   p1.y = -100;
   p1.x = 175 * sin(angle * D2R) + 175; //between x=0 and x=350

   //composite animation: Horizontal Motion
   p2.y = 50 * sin(6 * angle * D2R) - 225;
   p2.x = 400 * sin(angle * D2R) ; 

   //to refresh the window it calls display() function
   glutPostRedisplay(); //display()

}
#endif

void init() {
   //smoothing shapes
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char *argv[]) {

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Lab10(800x600) Harmonic Motions");

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
