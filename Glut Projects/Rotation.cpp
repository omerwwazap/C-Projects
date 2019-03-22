#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON      1  // 0:disable timer, 1:enable timer

#define D2R 0.0174532

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
  double x, y ;
} point_t ;

typedef struct {
   point_t pos;
   double angle ;
   double radius; // circular collision boundary
   bool hit;      // collision state
} player_t ;

typedef struct {
   point_t pos;
   double angle ;
   unsigned char r,g,b ; 
   double radius; // circular collision boundary
   bool hit;      // collision state
} entity_t ;

#define MAX 20 //maximum number of icons

entity_t icons[MAX] ;
int count = 0 ; 

player_t p = { {0,0} , 45,  20} ; 

//
// to draw circle, center at (x,y)
//  radius r
//
void circle( int x, int y, int r )
{
   double angle ;
   glBegin( GL_POLYGON ) ;
   for ( int i = 0 ; i < 360 ; i+=5 )
   {
      angle = i * D2R ;
      glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
   }
   glEnd();
}
void circle_wire(int x, int y, int r)
{
   float angle;
   glBegin(GL_LINE_LOOP);
   for (int i = 0; i < 360; i += 5)
   {
      angle = i * D2R;
      glVertex2f(x + r*cos(angle), y + r*sin(angle));
   }
   glEnd();
}
void vprint(int x, int y, void *font, char *string , ...)
{
   va_list ap;
   va_start ( ap, string );
   char str[1024] ;
   vsprintf_s( str, string, ap ) ;
   va_end(ap) ;
   
   int len, i ;
   glRasterPos2f( x, y );
   len = (int) strlen( str );
   for ( i =0; i<len; i++ )
   {
      glutBitmapCharacter( font, str[i]);
   }
}

// Apply translate (move) and rotation transformations
void vertex(point_t P, point_t Tr, double angle) {
   double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
   double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
   glVertex2d(xp, yp);
}


void drawPlayer( player_t p ) {
   double angle = p.angle * D2R ;
   glColor3f( 0.5, 0.5, 1.0 ) ;
   glBegin( GL_QUADS ) ;
   vertex({8, 25}, p.pos, angle);
   vertex({8, -25}, p.pos, angle);
   vertex({-8, -20}, p.pos, angle);
   vertex({-8, 20}, p.pos, angle);
   glEnd() ;
   glColor3f( 0.1, 0.1, 1.0 ) ;
   glLineWidth(2) ;
   glBegin( GL_LINE_LOOP ) ;
   vertex({8, 25}, p.pos, angle);
   vertex({8, -25}, p.pos, angle);
   vertex({-8, -20}, p.pos, angle);
   vertex({-8, 20}, p.pos, angle);
   glEnd() ;

   glColor3f( 0.8, 0.1, 0.1 ) ;
   circle( p.pos.x, p.pos.y, 15 ) ;

   glColor3f( 0.1, 0.1, 1.0 ) ;
   circle_wire( p.pos.x, p.pos.y, 15 ) ;
   glLineWidth( 1 ) ;

   // collision boundary
   if (p.hit) {
      glColor3f(0, 1, 0);
      circle_wire(p.pos.x, p.pos.y, p.radius);
   }

   glColor3f( 1,1,0 ) ; 
   vprint( p.pos.x -7, p.pos.y -5 , GLUT_BITMAP_HELVETICA_10,
          "%.0f", p.angle ) ;
}

void drawIcon( entity_t e ) {
   glColor3ub( e.r, e.g, e.b ) ;
   glBegin( GL_TRIANGLES ) ;
      vertex({ 0, 20 }, e.pos, e.angle);
      vertex({ -17.3, -10 }, e.pos, e.angle);
      vertex({ 17.3, -10 }, e.pos, e.angle);
   glEnd() ;
   glColor3f(0,0,0 ) ;
   glLineWidth(2) ;
   glBegin( GL_LINE_LOOP ) ;
      vertex({ 0, 20 }, e.pos, e.angle);
      vertex({ -17.3, -10 }, e.pos, e.angle);
      vertex({ 17.3, -10 }, e.pos, e.angle);
   glEnd() ;
   glLineWidth(1) ;

   // collision boundary
   if (e.hit) {
      glColor3f(0, 1, 0);
      circle_wire(e.pos.x, e.pos.y, e.radius);
   }
}

void drawAllIcons() {
   for ( int i=0 ; i< count; i++ ) {
      drawIcon( icons[i] ) ; 
   }
}
// display text with variables.

//
// To display onto window using OpenGL commands
//
void display()
{
   //
   // clear window to black
   //
   glClearColor( 0.3, 0.3 , 0.3 , 0 );
   glClear( GL_COLOR_BUFFER_BIT );

   drawPlayer( p ) ;
   drawAllIcons() ;
   
   glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
      exit(0);
    
    // to refresh the window it calls display() function
   //glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
      exit(0);
    
   // to refresh the window it calls display() function
   //glutPostRedisplay() ;
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown( int key, int x, int y )
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
void onSpecialKeyUp( int key, int x, int y )
{
   // Write your codes here.
   switch (key) {
   case GLUT_KEY_UP: up = false; break;
   case GLUT_KEY_DOWN: down = false; break;
   case GLUT_KEY_LEFT: left = false; break;
   case GLUT_KEY_RIGHT: right = false; break;
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
void onClick( int button, int stat, int x, int y )
{
   // Write your codes here.
   if ( button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && count < MAX ) {
      icons[count].pos = { x - winWidth / 2.0 , winHeight / 2.0 - y };
      icons[count].r = rand() % 256 ;
      icons[count].g = rand() % 256 ;
      icons[count].b = rand() % 256 ;
      icons[count].radius = 20;
      count++;
   }

   // to refresh the window it calls display() function
   //glutPostRedisplay() ; 
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize( int w, int h )
{
   // To setup coordinate system
   winWidth = w;
   winHeight = h;
   glViewport( 0,0,w,h) ;
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
   glMatrixMode( GL_MODELVIEW);
   glLoadIdentity();

}

void onMoveDown( int x, int y ) {
   // Write your codes here.


   
   // to refresh the window it calls display() function   
   //glutPostRedisplay() ;
}

void onMove( int x, int y ) {
   // Write your codes here.


   
   // to refresh the window it calls display() function
   //glutPostRedisplay() ;
}

// distance between two points.
double dist( point_t p1, point_t p2 ) {
   return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)) ;
}

// if the player hits anyone of the icons, it returns true.
bool hitToIcons(player_t *p, entity_t icons[], int size) {
   for (int i = 0; i < size; i++) {
      double d = dist(p->pos, icons[i].pos);
      if (d <= (p->radius + icons[i].radius)) {
         icons[i].hit = true;
         p->hit = true;
      }
   }
   return p->hit;
}

void movePlayer(player_t *p, double speed) {
   double dx = speed * cos(p->angle * D2R);
   double dy = speed * sin(p->angle * D2R);
   point_t oldPos = p->pos;
   p->pos = { p->pos.x + dx, p->pos.y + dy };
   // if the player hits any icons, do not move
   if ( hitToIcons( p, icons, count)) {
      p->pos = oldPos;
   }
   // check rectangular boundary
   if ( p->pos.x > (300 - p->radius) || p->pos.x < (-300 + p->radius) ||
        p->pos.y > (300 - p->radius) || p->pos.y < (-300 + p->radius) ) {
      p->pos = oldPos;
   }
}

void turnPlayer(player_t *p, double turn) {
   p->angle += turn;
   if (p->angle < 0) p->angle += 360;
   if (p->angle >= 360) p->angle -= 360;
}

// At the beginning of a frame, none of the objects are colliding.
void resetCollision() {
   p.hit = false;
   for (int i = 0; i < count; i++) {
      icons[i].hit = false;
   }
}

#if TIMER_ON == 1
void onTimer( int v ) {
    
   glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
   // Write your codes here.

   resetCollision();
   // turn the player clockwise direction
   if ( right ) {
      turnPlayer(&p, -4);
   }

   // turn the player counter-clockwise direction
   if ( left ) {
      turnPlayer(&p, 4);
   }

   // move forward
   if ( up ) {
      movePlayer(&p, 5);
   }

   // move backward
   if ( down ) {
      movePlayer(&p, -5);
   }

   // Find the nearest Icon and rotate it.
   if ( count > 0 ) {
      int nearest = 0 ;
      double minD = dist( p.pos, icons[0].pos ) ;
      for ( int i=1; i < count; i++ ) {
         double d = dist( p.pos, icons[i].pos ) ;
         if ( d < minD ) {
            minD = d ;
            nearest = i ;
         }
      }
      
      // update the nearest icon's angle
      icons[nearest].angle += 2;
      
   }
    
    // to refresh the window it calls display() function
   glutPostRedisplay() ; // display()

}
#endif

void main( int argc, char *argv[] )
{

   glutInit(&argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutCreateWindow( "Rotation" ) ;

   glutDisplayFunc( display ) ;
   glutReshapeFunc( onResize );
   //
   // keyboard registration
   //
   glutKeyboardFunc( onKeyDown ) ;
   glutSpecialFunc( onSpecialKeyDown ) ;

   glutKeyboardUpFunc( onKeyUp ) ;
   glutSpecialUpFunc( onSpecialKeyUp ) ;

   //
   // mouse registration
   //
   glutMouseFunc( onClick) ;
   glutMotionFunc( onMoveDown ) ;
   glutPassiveMotionFunc( onMove ) ; 
   
   #if  TIMER_ON == 1
   // timer event
   glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
   #endif

   glutMainLoop();
}
