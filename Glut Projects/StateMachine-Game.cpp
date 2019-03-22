//StateMachine-Game

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

/* TEMPLATE CONSTANTS AND VARIABLES */
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define  TIMER_PERIOD  300 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


/* USER GLOBAL VARIABLES and CONSTANTS */
#define  BLOCK_NUMBERS  30
#define  MIN_TIME_PERIOD    400
#define  TIME_DECREMENT    50

#define LOAD 0
#define MENU 1
#define RUN 2
#define GAMEOVER 3

int cx, cy;  // randomly selected cells coordinate.
int mx, my;  // current cell of the mouse position
int score;   // how many times the user clicked the popped cell.
int counter; // how many cells are popped out randomly.
bool active; // prevent more than one click on the same cell.
int timer;   // set the speed of game.

int gameState = LOAD;
int dotCount = 0;
int elapsedTime = 0;

//
// Intialize global variables.
//
void Init_Globals() {
   score = 0;
   counter = 0;
   timer = 300;
   // Initially, make the current selection outside of the window.
   cx = -100;
   cy = -100;
   mx = -100;
   my = -100;
   active = true;

}


//
// to draw circle, center at (x,y)
//  radius r
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
   for (i = 0; i<len; i++)
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
   for (i = 0; i<len; i++)
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
   for (i = 0; i<len; i++)
   {
      glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
   }
   glPopMatrix();
}

void drawGrid() {
   glColor3f(0.6, 0.6, 0.6);
   for (int i = -4; i <= 4; i++) {
      glBegin(GL_LINES);
      glVertex2f(50 * i, -200);
      glVertex2f(50 * i, 200);

      glVertex2f(-200, 50 * i);
      glVertex2f(200, 50 * i);
      glEnd();
   }
}

void drawCell() {
   int sx = cx * 50;
   int sy = cy * 50;
   glColor3f(0.9, 0.9, 0.9);
   glRectf(sx + 3, sy + 3, sx + 46, sy + 46);
}

void borderCell() {
   if (mx <= 3 && mx >= -4 && my <= 3 && my >= -4) {
      int posx = mx * 50;
      int posy = my * 50;
      glColor3f(0.7, 0.7, 0);
      glBegin(GL_LINE_LOOP);
      glVertex2f(posx, posy);
      glVertex2f(posx, posy + 50);
      glVertex2f(posx + 50, posy + 50);
      glVertex2f(posx + 50, posy);
      glEnd();
   }
}

void display_load() {
   vprint2(-50, -50, 0.2, "LOADING");
   for (int i = 0; i < dotCount; i++) {
      vprint2(80 + i * 20, -50, 0.2, ".");
   }
}

void display_menu() {
   glColor3f(1, 1, 0);
   vprint(-30, 100, GLUT_BITMAP_TIMES_ROMAN_24, "MENU");
   glColor3f(1, 1, 1);

   vprint(-50, 50, GLUT_BITMAP_HELVETICA_18, "START GAME");
   vprint(-50, 20, GLUT_BITMAP_HELVETICA_18, "OPTION");
   vprint(-50, -10, GLUT_BITMAP_HELVETICA_18, "EXIT");

   glColor3f(0, 1, 0);
   vprint(-100, -230, GLUT_BITMAP_9_BY_15, "SpaceBar to Start Game");
}

void display_run() {
   drawGrid();
   borderCell();
   drawCell();
   vprint(-240, -240, GLUT_BITMAP_9_BY_15, "Score: %d/%d", score, counter);
}

void display_gameover() {
   glColor3f(1, 1, 0);
   vprint(-50, 0, GLUT_BITMAP_9_BY_15, "Score: %d/%d", score, counter);
   glColor3f(1, 0.5, 0.5);
   vprint(-70, 30, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");

   glColor3f(1, 1, 1);
   vprint(-70, -230, GLUT_BITMAP_8_BY_13, "- F5 to go to Menu -");
}

//
// To display onto window using OpenGL commands
//
void display()
{

   //
   // clear window to black
   //
   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   switch (gameState) {
   case LOAD: display_load(); break;
   case MENU: display_menu(); break;
   case RUN: display_run(); break;
   case GAMEOVER: display_gameover(); break;
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

   // Start Game when the user presses spacebar in Menu.
   if (key == ' ' && gameState == MENU) {
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
   // Write your codes here.
   switch (key) {
   case GLUT_KEY_UP: up = true; break;
   case GLUT_KEY_DOWN: down = true; break;
   case GLUT_KEY_LEFT: left = true; break;
   case GLUT_KEY_RIGHT: right = true; break;

   }
   // Go back to Menu.
   if (key == GLUT_KEY_F5 && gameState == GAMEOVER) {
      Init_Globals();  // restart game.
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
void onClick(int button, int state, int x, int y)
{
   // Write your codes here.

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      // if the user clicks on the random block, increase score by 1.
      if (mx == cx && my == cy && active == true && gameState == RUN) {
         score++;
         active = false; // prevent more than one click for the same block.
      }
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

void onMove(int x, int y) {
   // Write your codes here.

   // from GLUT to OpenGL coordinate
   x = x - winWidth / 2;
   y = winHeight / 2 - y;

   // from OpenGL to Block coordinate
   mx = floor(x / 50.0f);
   my = floor(y / 50.0f);


   // to refresh the window it calls display() function
   glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

   glutTimerFunc(timer, onTimer, 0);
   // Write your codes here.

   if (gameState == LOAD) {
      dotCount = (dotCount + 1) % 4;
      elapsedTime++;
      if (elapsedTime == 8) {
         gameState = MENU;
      }
   }
   else if (gameState == RUN) {
      if (counter == BLOCK_NUMBERS) {
         gameState = GAMEOVER;
      }
      else {

         cx = rand() % 8 - 4;
         cy = rand() % 8 - 4;
         active = true;

         if (timer > MIN_TIME_PERIOD) {
            timer -= TIME_DECREMENT;
         }

         counter++;
      }
   }



   // to refresh the window it calls display() function
   glutPostRedisplay(); // display()

}
#endif

void Init() {
   Init_Globals();
}


void main(int argc, char *argv[])
{

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