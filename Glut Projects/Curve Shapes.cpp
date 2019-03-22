
#include GLglut.h
#include stdio.h
#include stdlib.h
#include math.h
#include string.h
#include stdarg.h

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 400

#define TIMER_PERIOD 1000 period for the timer
#define TIMER_ON        0 0disable timer, 1enable timer

#define D2R 0.0174532

 Global Variables for Template File 
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; current Window width and height

void myCircle_wire(float x, float y, float r) {
   glBegin(GL_LINE_LOOP);
   for (float angle = 0; angle  360; angle += 10) {
      glVertex2f(r  cos(angleD2R) + x, r  sin(angleD2R) + y);
   }
   glEnd();
}

void myCircle_filled(float x, float y, float r) {
   glBegin(GL_POLYGON);
   for (float angle = 0; angle  360; angle += 10) {
      glVertex2f(r  cos(angleD2R) + x, r  sin(angleD2R) + y);
   }
   glEnd();
}

void pie_wire(float x, float y, float r, float start, float end) {
   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   for (float angle = start; angle  end; angle += 10) {
      glVertex2f(r  cos(angleD2R) + x, r  sin(angleD2R) + y);
   }
   glVertex2f(r  cos(endD2R) + x, r  sin(endD2R) + y);
   glEnd();
}

void pie_filled(float x, float y, float r, float start, float end) {
   glBegin(GL_POLYGON);
   glVertex2f(x, y);
   for (float angle = start; angle  end; angle += 10) {
      glVertex2f(r  cos(angleD2R) + x, r  sin(angleD2R) + y);
   }
   glVertex2f(r  cos(endD2R) + x, r  sin(endD2R) + y);
   glEnd();
}

void spiral(float x, float y, float r, float start, float end, float offset) {
   glBegin(GL_LINE_STRIP);
   for (float angle = start; angle  end; angle += 10) {
      r -= offset; 
      glVertex2f(r  cos(angleD2R) + x, r  sin(angleD2R) + y);
   }
   glEnd();
}


   to draw circle, center at (x,y) radius r
   DAIRE

void circle(int x, int y, int r) {
#define PI 3.1415
   float angle;
   glBegin(GL_POLYGON);
   for (int i = 0; i  100; i++) {
      angle = 2  PIi  100;
      glVertex2f(x + rcos(angle), y + rsin(angle));
   }
   glEnd();
}


   CEMBER

void circle_wire(int x, int y, int r) {
#define PI 3.1415
   float angle;

   glBegin(GL_LINE_LOOP);
   for (int i = 0; i  100; i++) {
      angle = 2  PIi  100;
      glVertex2f(x + rcos(angle), y + rsin(angle));
   }
   glEnd();
}

void print(int x, int y, char string, void font) {
   int len, i;

   glRasterPos2f(x, y);
   len = (int)strlen(string);
   for (i = 0; ilen; i++) {
      glutBitmapCharacter(font, string[i]);
   }
}


   to display text with variables
vprint(-winWidth  2 + 10, winHeight  2 - 20, GLUT_BITMAP_8_BY_13, ERROR %d, numClicks);

void vprint(int x, int y, void font, char string, ...) {
   va_list ap;
   va_start(ap, string);
   char str[1024];
   vsprintf_s(str, string, ap);
   va_end(ap);

   int len, i;
   glRasterPos2f(x, y);
   len = (int)strlen(str);
   for (i = 0; ilen; i++) {
      glutBitmapCharacter(font, str[i]);
   }
}


   vprint2(-50, 0, 0.35, 00%02d, timeCounter);

void vprint2(int x, int y, float size, char string, ...) {
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
   for (i = 0; ilen; i++) {
      glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
   }
   glPopMatrix();
}


   to display onto window using OpenGL commands

void display() {
   clear window to black
   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   filled and line circles
   glColor3f(1, 1, 1);
   myCircle_wire(-100, 60, 50);

   glColor3f(1, 0, 0);
   myCircle_filled(-100, 60, 40);

   pie charts
   glColor3f(1, 1, 0);
   pie_filled( 100, -100, 60, 90, 220);

   glColor3f(1, 0.5, 0.5);
   pie_filled(100, -100, 60, -30, 60);

   glColor3f(0.5, 0.5, 1.0);
   pie_filled(100, -100, 60, 230, 300);

   pacman
   glColor3f(1, 1, 0);
   pie_filled(-50, -100, 50, 30, 330);

   glColor3f(0, 0, 0);
   myCircle_filled(-40, -70, 5);

   spiral path
   glColor3f(0.5, 0.5, 0.5);
   spiral(100, 100, 60, 0, 1080, 0.5);

   glutSwapBuffers();
}


   key function for ASCII charachters like ESC, a,b,c..,A,B,..Z

void onKeyDown(unsigned char key, int x, int y) {
   exit when ESC is pressed.
   if (key == 27)
      exit(0);

   to refresh the window it calls display() function
   glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
   exit when ESC is pressed.
   if (key == 27)
      exit(0);

   to refresh the window it calls display() function
   glutPostRedisplay();
}


   special Key like GLUT_KEY_F1, F2, F3,...
   arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT

void onSpecialKeyDown(int key, int x, int y) {
   write your codes here.

   switch (key) {

   case GLUT_KEY_UP
      up = true;
      break;

   case GLUT_KEY_DOWN
      down = true;
      break;

   case GLUT_KEY_LEFT
      left = true;
      break;

   case GLUT_KEY_RIGHT
      right = true;
      break;

   }

   to refresh the window it calls display() function
   glutPostRedisplay();
}


   special Key like GLUT_KEY_F1, F2, F3,...
   arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT

void onSpecialKeyUp(int key, int x, int y) {
   write your codes here

   switch (key) {

   case GLUT_KEY_UP
      up = false;
      break;

   case GLUT_KEY_DOWN
      down = false;
      break;

   case GLUT_KEY_LEFT
      left = false;
      break;

   case GLUT_KEY_RIGHT
      right = false;
      break;
   }

   to refresh the window it calls display() function
   glutPostRedisplay();
}


   When a click occurs in the window,
   It provides which button
   buttons  GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
   states   GLUT_UP , GLUT_DOWN
   x, y is the coordinate of the point that mouse clicked

void onClick(int button, int stat, int x, int y) {
   write your codes here.



   to refresh the window it calls display() function
   glutPostRedisplay();
}


   This function is called when the window size changes.
   w  is the new width of the window in pixels
   h  is the new height of the window in pixels

void onResize(int w, int h) {
   winWidth = w;
   winHeight = h;
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-w  2, w  2, -h  2, h  2, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   display(); refresh window
}

void onMoveDown(int x, int y) {
   write your codes here.



   to refresh the window it calls display() function	
   glutPostRedisplay();
}


   GLUT to OpenGL coordinate conversion
   x2 = x1 - winWidth  2
   y2 = winHeight  2 - y1

void onMove(int x, int y) {
   write your codes here.



   to refresh the window it calls display() function
   glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
   glutTimerFunc(TIMER_PERIOD, onTimer, 0);
   write your codes here



   to refresh the window it calls display() function
   glutPostRedisplay(); display()

}
#endif

void init() {
   smoothing shapes
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char argv[]) {

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB  GLUT_DOUBLE);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(1800, 100);
   glutCreateWindow(Lab09(400x400) Trigonometry - Curve Shapes);

   glutDisplayFunc(display);
   glutReshapeFunc(onResize);

   keyboard registration
   glutKeyboardFunc(onKeyDown);
   glutSpecialFunc(onSpecialKeyDown);

   glutKeyboardUpFunc(onKeyUp);
   glutSpecialUpFunc(onSpecialKeyUp);

   mouse registration
   glutMouseFunc(onClick);
   glutMotionFunc(onMoveDown);
   glutPassiveMotionFunc(onMove);

#if TIMER_ON == 1
    timer event
   glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

   init();

   glutMainLoop();

}
