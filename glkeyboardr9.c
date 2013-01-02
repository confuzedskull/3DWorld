#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


bool* keyStates = new bool[256]; // Create an array of boolean values of letters
int window_width=640;
int window_height=320;

struct limit{float xmin; float xmax; float ymin; float ymax; float zmin; float zmax;};
struct object{float x; float y; float z; float xrot; float yrot; float resting; float radius; limit limits; int action; int status;};


int temp;

float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float xrot=0.0f, y_rotation=0.0f;
float lastx, lasty;

float color=0.0f;
float cubesize=2.0f;
float togglecube=0.0f;
int togglehold=0;
int hold=0;
float resting=-1.0;

const float piover180 = 0.0174532925f;
float heading;

char text1[256];
char text2[256];
char text3[256];
char text4[256];
char text5[256];
char text6[256];
char text7[256];
char text8[256];
char text9[256];
char text10[256];
char text11[256];
char text12[256];
char text13[256];
char text14[256];
char text15[256];
char text16[256];
char text17[256];
char text18[256];
char text19[256];


float xpos=0;
float ypos=0;
float zpos=-8;
object player;

int standing=0;
int jumping=1;
int crouching=0.5;
int falling=-1;
int movingforward=8;
int movingbackward=2;
int movingleft=4;
int movingright=6;

object cube;
object cube2;
object mod;
object wall_1;
object wall_2;
object wall_3;
object wall_4;

int max_height=player.resting+4;

int held;

GLfloat	yrot=0.0;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;

float angle = 0.0f;
float playertocube;


float distance(int x1, int y1, int x2, int y2)
{
    return (sqrt(pow((x2 - x1),2) + pow((y2 - y1),2)));
}

bool touching_top(object object1, object object2)
{

    if (object1.limits.ymin>=object2.limits.ymax
        && object1.x<object2.limits.xmax && object1.x>object2.limits.xmin
        && object1.z<object2.limits.zmax && object1.z>object2.limits.zmin)
    return true;
    else
    return false;
}

bool touching_side(object object1, object object2)
{

    if (object1.y<object2.limits.ymax && object1.y>object2.limits.ymin
        && object1.x<=object2.limits.xmax && object1.x>=object2.limits.xmin
        && object1.z<=object2.limits.zmax && object1.z>=object2.limits.zmin)
    return true;
    else
    return false;
}


bool touching(object object1, object object2)
{

    if (distance(object1.x,object1.z,object2.x, object2.z) <= object1.radius+object2.radius)
    return true;
    else
    return false;
}

void holding (object B)
{   float yrotrad, xrotrad;
    yrotrad = (player.yrot / 180 * 3.141592654f);
    xrotrad = (player.xrot / 180 * 3.141592654f);
    if (togglehold==1)
    {
        B.x = player.x-(float)sin(yrotrad) * 0.5f;
        B.z = player.z+(float)cos(yrotrad) * 0.5f;
        B.y -= float(sin(xrotrad));
        B.action=held;
    }
}

void gravity(object A)
{
    if (A.limits.ymin>A.resting)
    {
        ypos-=0.04;
        player.action=falling;
    }



}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

        // Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

   void enable (void) {
    glEnable (GL_DEPTH_TEST); //enable the depth testing
    //glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader

}

void glutPrint(float x, float y, LPVOID font, char* text, float r, float g, float b, float a)
{
    if(!text || !strlen(text)) return;
    bool blending = false;
    if(glIsEnabled(GL_BLEND)) blending = true;
    glEnable(GL_BLEND);
    glColor4f(r,g,b,a);
    glRasterPos2f(x,y);


    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
    if(!blending) glDisable(GL_BLEND);
}

void ViewOrtho(int x, int y)							// Set Up An Ortho View
{
	glMatrixMode(GL_PROJECTION);					// Select Projection
	glPushMatrix();							// Push The Matrix
	glLoadIdentity();						// Reset The Matrix
	glOrtho( 0, x , y , 0, -1, 1 );				// Select Ortho Mode
	glMatrixMode(GL_MODELVIEW);					// Select Modelview Matrix
	glPushMatrix();							// Push The Matrix
	glLoadIdentity();						// Reset The Matrix
}

void ViewPerspective(void)							// Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );					// Select Projection
	glPopMatrix();							// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );					// Select Modelview
	glPopMatrix();							// Pop The Matrix
}

void playerinit(void)
{
    player.x=xpos;
    player.y=ypos;
    player.z=zpos;
    //player.xrot=xrot;
    player.yrot=360-yrot;
    player.resting=-1.0;
    player.action=standing;
    player.radius=1.0;
    player.limits.ymin=player.y-1;
    player.limits.ymax=player.y+1;
    player.limits.xmin=player.x-0.5;
    player.limits.xmax=player.x+0.5;
    player.limits.zmin=player.z-0.5;
    player.limits.zmax=player.z+0.5;

    //holding(cube);

    gravity(player);

}

void cubeinit(void)
{
    cube.x=0.0;
    cube.y=-4.0;
    cube.z=0.0;
    cube.yrot=angle;
    cube.resting=0.0;
    cube.action=standing;
    cube.radius=1.0;
    cube.limits.xmin=mod.x-cube.radius;
    cube.limits.xmax=mod.x+cube.radius;
    cube.limits.ymin=mod.y-cube.radius;
    cube.limits.ymax=mod.y+cube.radius;
    cube.limits.zmin=mod.z-cube.radius;
    cube.limits.zmax=mod.z+cube.radius;

    if (cube.limits.ymin>cube.resting)
    cube.y-0.03;

}

void cube2init(void)
{
    cube2.x=8.0;
    cube2.y=0.0;
    cube2.z=8.0;
    cube2.resting=0.0;
    cube2.action=standing;
    cube2.radius=2.0;
    cube2.limits.xmin=cube2.x-cube2.radius;
    cube2.limits.xmax=cube2.x+cube2.radius;
    cube2.limits.ymin=cube2.y-cube2.radius;
    cube2.limits.ymax=cube2.y+cube2.radius;
    cube2.limits.zmin=cube2.z-cube2.radius;
    cube2.limits.zmax=cube2.z+cube2.radius;

}

void wall_init(void)
{
    wall_1.limits.xmin=-10;
    wall_1.limits.xmax=10;
    wall_1.limits.ymin=-1;
    wall_1.limits.ymax=5;
    wall_1.limits.zmin=9;
    wall_1.limits.zmax=11;
//yellow wall
    wall_2.limits.xmin=-10;
    wall_2.limits.xmax=10;
    wall_2.limits.ymin=-1;
    wall_2.limits.ymax=5;
    wall_2.limits.zmin=-11;
    wall_2.limits.zmax=-9;
//cyan wall
    wall_3.limits.xmin=-11;
    wall_3.limits.xmax=-9;
    wall_3.limits.ymin=-1;
    wall_3.limits.ymax=5;
    wall_3.limits.zmin=-10;
    wall_3.limits.zmax= 10;
//green wall
    wall_4.limits.xmin=9;
    wall_4.limits.xmax=11;
    wall_4.limits.ymin=-1;
    wall_4.limits.ymax=5;
    wall_4.limits.zmin=-10;
    wall_4.limits.zmax=10;

}


void keyPressed (unsigned char key, int x, int y)
{
keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp (unsigned char key, int x, int y)
{
keyStates[key] = false; // Set the state of the current key to not pressed
}

void keyOperations (void)
{

if (keyStates['w'] &&
    touching_side(player,cube)==false &&
    touching_side(player,cube2)==false &&
    touching_side(player,wall_1)==false &&
    touching_side(player,wall_2)==false &&
    touching_side(player,wall_3)==false &&
    touching_side(player,wall_4)==false)
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos -= (float)sin(yrotrad) * 0.01f;
        zpos += (float)cos(yrotrad) * 0.01f;
        if(walkbiasangle >= 359.0f)
        {
            walkbiasangle = 0.0f;
        }
        else
        {
            walkbiasangle+= 10;
        }
        walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
        player.action=movingforward;
    }

if (keyStates['s'])
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos += (float)sin(yrotrad) * 0.01f;
        zpos -= (float)cos(yrotrad) * 0.01f;
        if(walkbiasangle <= 1.0f)
        {
            walkbiasangle = 359.0f;
        }
        else
        {
            walkbiasangle-= 10;
        }
        walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
        player.action=movingbackward;
    }

if (keyStates['a'] &&
    touching_side(player,cube)==false &&
    touching_side(player,cube2)==false &&
    touching_side(player,wall_1)==false &&
    touching_side(player,wall_2)==false &&
    touching_side(player,wall_3)==false &&
    touching_side(player,wall_4)==false)
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos -= (float)cos(yrotrad) * 0.01f;
        zpos -= (float)sin(yrotrad) * 0.01f;
        if(walkbiasangle <= 1.0f)
        {
            walkbiasangle = 359.0f;
        }
        else
        {
            walkbiasangle-= 10;
        }
        walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
        player.action=movingleft;
    }

if (keyStates['d'] &&
    touching_side(player,cube)==false &&
    touching_side(player,cube2)==false &&
    touching_side(player,wall_1)==false &&
    touching_side(player,wall_2)==false &&
    touching_side(player,wall_3)==false &&
    touching_side(player,wall_4)==false)
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos += (float)cos(yrotrad) * 0.01f;
        zpos += (float)sin(yrotrad) * 0.01f;
        if(walkbiasangle >= 359.0f)
        {
            walkbiasangle = 0.0f;
        }
        else
        {
            walkbiasangle+= 10;
        }
        walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
        player.action=movingright;
    }

if (keyStates['e'] && touching(player,cube))
    {

        if(togglehold==0)
        hold=1;
        if(togglehold==1)
        hold=0;


    }
if (keyStates['e']==false)
    togglehold=hold;



if (keyStates[32] /*&& player.action!=falling*/)
    {
        player.action=jumping;
    }

/*
if (keyStates[32]==false)
{
    player.action=mod.action;
}
*/
if (keyStates[27])
		exit(0);
}

void mouse_enter(int state)
{
    if (state==GLUT_ENTERED)
    glutWarpPointer(window_width / 2, window_height / 2);
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position

    //if(player.xrot>90 && player.xrot<270)
    player.xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position

    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position

    /*if(x>=window_width||x<=-window_width || y>=window_height||y<=-window_height)
    glutWarpPointer(window_width / 2, window_height / 2);*/
}

void text (void)
{
    int scaled_xpos=window_width*0.75;

    sprintf(text1,"x=%f",player.x);
    sprintf(text2,"y=%f",player.y);
    sprintf(text3,"z=%f",player.z);
    sprintf(text4, "x rotation: %f", player.xrot);
    sprintf(text5, "y rotation: %f", player.yrot);
    sprintf(text6, "togglehold: %d", togglehold);
    sprintf(text7, "cubex: %f", cube.x);
    sprintf(text8, "cubey: %f", cube.y);
    sprintf(text9, "cubez: %f", cube.z);
    sprintf(text10, "distance: %f", playertocube);
    sprintf(text11, "player action: %d", player.action);
    sprintf(text12, "player resting: %f",player.resting);
    sprintf(text13, "cube xmin: %f", cube.limits.xmin);
    sprintf(text14, "cube xmax: %f", cube.limits.xmax);
    sprintf(text19, "cube ymin: %f", cube.limits.ymin);

    sprintf(text15, "cube ymax: %f", cube.limits.ymax);
    sprintf(text16, "player ymin: %f", player.limits.ymin);
    sprintf(text17, "cube zmin: %f", cube.limits.zmin);
    sprintf(text18, "cube zmax: %f", cube.limits.zmax);


    glutPrint (0.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text1, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,20.0f, GLUT_BITMAP_HELVETICA_12, text2, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,30.0f, GLUT_BITMAP_HELVETICA_12, text3, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,40.0f, GLUT_BITMAP_HELVETICA_12, text4, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,50.0f, GLUT_BITMAP_HELVETICA_12, text5, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,60.0f, GLUT_BITMAP_HELVETICA_12, text6, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,10.0f, GLUT_BITMAP_HELVETICA_12, text7, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,20.0f, GLUT_BITMAP_HELVETICA_12, text8, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,30.0f, GLUT_BITMAP_HELVETICA_12, text9, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (200.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text10, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,70.0f, GLUT_BITMAP_HELVETICA_12, text11, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (0.0f,80.0f, GLUT_BITMAP_HELVETICA_12, text12, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,40.0f, GLUT_BITMAP_HELVETICA_12, text13, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,50.0f, GLUT_BITMAP_HELVETICA_12, text14, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,60.0f, GLUT_BITMAP_HELVETICA_12, text15, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (200.0f,40.0f, GLUT_BITMAP_HELVETICA_12, text16, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,70.0f, GLUT_BITMAP_HELVETICA_12, text17, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,80.0f, GLUT_BITMAP_HELVETICA_12, text18, 1.0f,0.0f,0.0f, 0.5f);
    glutPrint (scaled_xpos,90.0f, GLUT_BITMAP_HELVETICA_12, text19, 1.0f,0.0f,0.0f, 0.5f);
}

void renderScene(void) {

    float yrotrad, xrotrad;
    yrotrad = (player.yrot / 180 * 3.141592654f);
    xrotrad = (player.xrot / 180 * 3.141592654f);

    keyOperations();
    playerinit();
    cubeinit();
    cube2init();
    wall_init();
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	enable();

	ypos=player.y;

    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    GLfloat ytrans = ypos;


//Rotation restrictions
    if (player.xrot>179)
    player.xrot=-179;
    if (player.xrot<-179)
    player.xrot=179;
    glRotatef(player.xrot, 1.0f, 0.0f, 0.0f);
    if (player.yrot>359)
    player.yrot=-359;
    if (player.yrot<-359)
    player.yrot=359;
	glRotatef(player.yrot, 0.0f, 1.0f, 0.0f);


    if (touching_top(player,cube)==true)
    {
        mod.resting=cube.limits.ymax;
    }
    else
    mod.resting=player.resting;

    player.resting=mod.resting;


    if (touching_top(player,cube2)==true)
    mod.resting=cube2.limits.ymax;
    else
    mod.resting=player.resting;

    player.resting=mod.resting;

gravity(player);
    if (player.resting>=cube.limits.ymax)
    ypos=player.resting+1;


    if (player.resting>=cube2.limits.ymax)
    ypos=player.resting+1;

     if (player.action==jumping)
    {
        ypos+=4;
    }


    if (cube.limits.ymin>0)
    cube.y-0.03;

    glTranslatef(xtrans, ytrans, ztrans);
    glPushMatrix();
    glScalef(5.0,2.5,5.0);
    glTranslatef(0.0,-1.25,0.0);
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, 2.0f);
		glVertex3f( 2.0f, 2.0f, 2.0f);
    glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(2.0f, -2.0f, 2.0f);
		glVertex3f( -2.0f, -2.0f, 2.0f);
		glVertex3f( -2.0f, -2.0f, -2.0f);
		glVertex3f( 2.0f, -2.0f, -2.0f);
    glColor3f(0.5f,0.0f,1.0f);
		glVertex3f(2.0f, 2.0f, 2.0f);
		glVertex3f( -2.0f, 2.0f, 2.0f);
		glVertex3f( -2.0f, -2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, 2.0f);
    glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(2.0f, -2.0f, -2.0f);
		glVertex3f( -2.0f, -2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, -2.0f);
		glVertex3f( 2.0f, 2.0f, -2.0f);
    glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(-2.0f, 2.0f, 2.0f);
		glVertex3f( -2.0f, 2.0f, -2.0f);
		glVertex3f( -2.0f, -2.0f, -2.0f);
		glVertex3f( -2.0f, -2.0f, 2.0f);
    glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f( 2.0f, 2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, -2.0f);
	glEnd();
	glPopMatrix();
//small cube//

glPushMatrix();

if (togglehold==1)
    {
        mod.x = player.x-(float)sin(yrotrad) * 2;
        mod.z = player.z+(float)cos(yrotrad) * 2;
        mod.y = player.y;//-(float)sin(xrotrad) * 1.5;
        mod.yrot = -player.yrot;

    }

angle=mod.yrot;
cube.x=mod.x;
cube.y=-mod.y;
cube.z=mod.z;
cube.yrot=mod.yrot;
glutPostRedisplay();
glTranslatef(cube.x,cube.y,cube.z);
glRotatef(angle, 0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);


glutPostRedisplay();
	glColor3f(color,color,color);
		glVertex3f(0.5f, 0.0f, -0.5f);
		glVertex3f( -0.5f, 0.5f, -0.5f);
		glVertex3f( -0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
    //glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, -0.5f,0.5f);
		glVertex3f( -0.5f, -0.5f, 0.5f);
		glVertex3f( -0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
    //glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f( -0.5f, 0.5f, 0.5f);
		glVertex3f( -0.5f, -0.5f, 0.5f);
		glVertex3f( 0.5f, -0.5f, 0.5f);
    //glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f( -0.5f, -0.5f, -0.5f);
		glVertex3f( -0.5f, 0.5f, -0.5f);
		glVertex3f( 0.5f, 0.5f, -0.5f);
    //glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f( -0.5f, 0.5f, -0.5f);
		glVertex3f( -0.5f, -0.5f, -0.5f);
		glVertex3f( -0.5f, -0.5f, 0.5f);
    //glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, -0.5f, 0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
	glEnd();
glPopMatrix();

cube2init();
glPushMatrix();
glTranslatef(cube2.x, cube2.y, cube2.z);
glColor3f(1.0,0.0,1.0);
glutSolidCube(4);
glutPostRedisplay();
glPopMatrix();



glPushMatrix();
	ViewOrtho(window_width,window_height);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f,0.0,0.0f);
    //Crosshair:
    glBegin(GL_LINES);
    //Vertical line
        glVertex2f((window_width/2), (window_height/2 - 10));
        glVertex2f((window_width/2), (window_height/2 + 10));
    //Horizontal line
        glVertex2f((window_width/2 + 10), (window_height/2));
        glVertex2f((window_width /2 - 10), (window_height/2));
    glEnd();

glPushMatrix();
playertocube=distance(player.x,player.z,cube.x, cube.z);
text();


glPopMatrix();

    ViewPerspective();

//glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char **argv) {

	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(window_width,window_height);
	glutCreateWindow("3D World");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key
	glutSetCursor(GLUT_CURSOR_NONE);
    glutEntryFunc(mouse_enter);
    glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
