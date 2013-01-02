#include <stdio.h>
#include <stdlib.h>
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
struct object{float x; float y; float z; float xrot; float yrot; float resting; float limits; int action; int status;};

float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float xrot=0.0f, y_rotation=0.0f;
float lastx, lasty;

float color=0.0f;
float cubesize=2.0f;
float togglecube=0.0f;
int togglehold=0;
int hold=0;

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

float xpos=0;
float ypos=0;
float zpos=-10;
object player;

int standing;
int jumping;
int falling;

object cube;
object mod;

int held;

GLfloat	yrot=0.0;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;

float angle = 0.0f;
float playertocube;


int distance(int x1, int y1, int x2, int y2)
{
    return (sqrt(pow((x2 - x1),2) + pow((y2 - y1),2)));
}

bool touching(object object1, object object2)
{

    if (distance(object1.x,object1.z,object2.x, object2.z) <= object1.limits+object2.limits)
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
    if (A.y>A.resting)
    {   A.y-=0.03;
        ypos-=0.03;
        A.action=falling;
    }
    else
    {
        A.resting=A.y;
        A.action=standing;
    }
}

void touchcube()
{
    if(distance(xpos,zpos,cube.x,cube.z) <= player.limits+cube.limits)
    {

        if (color==0.0)
        {
        togglecube=1.0f;
        }
        if (color==1)
        togglecube=0.0;
        cubesize++;
    }
    if(distance(xpos,zpos,cube.x,cube.z) > player.limits+cube.limits)
    {
        color=togglecube;
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
    player.resting=2.0;
    player.action=standing;
    player.limits=1.0;
    gravity(player);
    //holding(cube);
}

void cubeinit(void)
{
    cube.x=0.0;
    cube.y=-4.0;
    cube.z=0.0;
    cube.yrot=angle;
    cube.resting=0.0;
    cube.action=standing;
    cube.limits=1.0;

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

if (keyStates['w'])
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
    }

if (keyStates['a'])
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
    }

if (keyStates['d'])
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



if (keyStates[32])
    {
        if(player.y<player.resting+2)
        {
            ypos+=2;

        }
    }
if (keyStates[32]==false)
{
    player.action=falling;
}
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
    sprintf(text1,"x=%f",player.x);
    sprintf(text2,"y=%f",player.y);
    sprintf(text3,"z=%f",player.z);
    sprintf(text4, "x rotation: %f", player.xrot);
    sprintf(text5, "y rotation: %f", player.yrot);
    sprintf(text6, "togglehold: %d", togglehold);
    sprintf(text7, "cubex: %f", mod.x);
    sprintf(text8, "cubey: %f", cube.y);
    sprintf(text9, "cubez: %f", mod.z);
    sprintf(text10, "distance: %f", playertocube);

    glutPrint (0.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text1, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,20.0f, GLUT_BITMAP_HELVETICA_12, text2, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,30.0f, GLUT_BITMAP_HELVETICA_12, text3, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,40.0f, GLUT_BITMAP_HELVETICA_12, text4, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,50.0f, GLUT_BITMAP_HELVETICA_12, text5, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,60.0f, GLUT_BITMAP_HELVETICA_12, text6, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (400.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text7, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (400.0f,20.0f, GLUT_BITMAP_HELVETICA_12, text8, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (400.0f,30.0f, GLUT_BITMAP_HELVETICA_12, text9, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (200.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text10, 1.0f,1.0f,1.0f, 0.5f);
}

void renderScene(void) {

    float yrotrad, xrotrad;
    yrotrad = (player.yrot / 180 * 3.141592654f);
    xrotrad = (player.xrot / 180 * 3.141592654f);

    keyOperations();
    playerinit();
    cubeinit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	enable();

    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    GLfloat ytrans = ypos;


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
    glColor3f(1.0f,0.0f,0.0f);
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
touchcube();

if (togglehold==1)
    {
        mod.x = player.x-(float)sin(yrotrad) * 2;
        mod.z = player.z+(float)cos(yrotrad) * 2;
        mod.y = player.y;//-(float)sin(xrotrad) * 1.5;
        mod.yrot = player.yrot;
        cube.action=held;
    }


cube.x=mod.x;
cube.y=-mod.y;
cube.z=mod.z;
cube.yrot=mod.yrot;
glutPostRedisplay();
gravity(cube);
glTranslatef(cube.x,cube.y,cube.z);
glRotatef(angle, 0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);

angle+=0.1f;
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
/*
glPushMatrix();
glTranslatef(8.0, 0.0, 8.0);
glColor3f(1.0,0.0,1.0);
glutSolidCube(4);
glutPostRedisplay();
glPopMatrix();

glPushMatrix();
glTranslatef(-8.0, 0.0, 8.0);
glColor3f(0.5,1.0,0.0);
glutSolidCube(4);
glutPostRedisplay();
glPopMatrix();
*/
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
