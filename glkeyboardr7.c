#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int window_width=640;
int window_height=320;
struct object{float x; float y; float z; float xrot; float yrot; float resting; int limits; int action; int status;};

float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float xrot=0.0f, y_rotation=0.0f;
float lastx, lasty;

float color=0.0f;
float size=1.0;
float togglecube=0.0f;

const float piover180 = 0.0174532925f;
float heading;

char text1[256];
char text2[256];
char text3[256];
char text4[256];
char text5[256];
char text6[256];

float xpos=0;
float ypos=0;
float zpos=-10;
object player;

int standing;
int jumping;
int falling;

float cubeposx=0.0f;
float cubeposy=0.0f;
float cubeposz=0.0f;
object cube;

GLfloat	yrot=0.0;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;

float angle = 0.0f;

int distance(int x1, int y1, int x2, int y2)
{
    return (sqrt(pow((x2 - x1),2) + pow((y2 - y1),2)));
}

bool touching(object object1, object object2)
{

    if (distance(object1.x,object1.z,object2.x, object2.z) < object1.limits+object2.limits)
    return true;
    else
    return false;
}

void gravity(object A)
{
    if (A.y>A.resting)
    {   A.y-=0.01;
        ypos-=0.005;
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
    if(distance(xpos,zpos,cubeposx,cubeposz) == player.limits+cube.limits)
    {
        if (color==0.0)
        {
        togglecube=1.0f;
        }
        if (color==1)
        togglecube=0.0;
    }
    if(distance(xpos,zpos,cubeposx,cubeposz) > player.limits+cube.limits)
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
    player.xrot=xrot;
    player.yrot=360-yrot;
    player.action=standing;
    player.limits=0.5;
}

void renderScene(void) {


    playerinit();
    cube.x=cubeposx;
    cube.z=cubeposz;
    cube.limits=1;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	enable();

    sprintf(text1,"x=%f",player.x);
    sprintf(text2,"y=%f",player.y);
    sprintf(text3,"z=%f",player.z);
    sprintf(text4, "x rotation: %f", player.xrot);
    sprintf(text5, "y rotation: %f", player.yrot);
    sprintf(text6, "action: %d", player.action);


    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    GLfloat ytrans = -walkbias-0.25f+ypos;

    if(xrot>=359)
    xrot=0;
    if(player.xrot>90 && player.xrot<270)
    glRotatef(player.xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(player.yrot, 0.0f, 1.0f, 0.0f);
    glTranslatef(xtrans, ytrans, ztrans);
   // glRasterPos3f(referencex, referencey, referencez);
    glPushMatrix();
    gravity(player);
    glScalef(5.0,2.5,5.0);
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
glTranslatef(0.0,0.0,2.0);
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
    glutPrint (0.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text1, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,20.0f, GLUT_BITMAP_HELVETICA_12, text2, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,30.0f, GLUT_BITMAP_HELVETICA_12, text3, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,40.0f, GLUT_BITMAP_HELVETICA_12, text4, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,50.0f, GLUT_BITMAP_HELVETICA_12, text5, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,60.0f, GLUT_BITMAP_HELVETICA_12, text6, 1.0f,1.0f,1.0f, 0.5f);

glPopMatrix();

    ViewPerspective();

//glPopMatrix();
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

if(touching(player,cube)==false)
{
    if (key == 'w')
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos -= (float)sin(yrotrad) * 0.05f;
        zpos += (float)cos(yrotrad) * 0.05f;
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
}
    if (key == 'a')
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos -= (float)cos(yrotrad) * 0.05f;
        zpos -= (float)sin(yrotrad) * 0.05f;
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

    if (key == 'd')
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos += (float)cos(yrotrad) * 0.05f;
        zpos += (float)sin(yrotrad) * 0.05f;
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

    if (key == 's')
    {
        float yrotrad;
        yrotrad = (player.yrot / 180 * 3.141592654f);
        xpos += (float)sin(yrotrad) * 0.05f;
        zpos -= (float)cos(yrotrad) * 0.05f;
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

    if (key == 32)
    {
        if(player.action==standing &&player.y<player.resting+2)
        {
            ypos+=2;
            player.y=ytrans;
            player.action=jumping;
        }
    }
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT :
                heading+=1.0f;
                yrot=heading;
                break;
		case GLUT_KEY_UP :
				xrot -=1.0;
                break;
		case GLUT_KEY_RIGHT :
				heading-=1.0f;
				yrot=heading;                                         ;
				break;
        case GLUT_KEY_DOWN :
				xrot +=1.0;
				break;
	}
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

    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position

    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position

    /*if(x>=window_width||x<=-window_width || y>=window_height||y<=-window_height)
    glutWarpPointer(window_width / 2, window_height / 2);*/
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(window_width,window_height);
	glutCreateWindow("3D World");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	// here are the new entries
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	//glutSetCursor(GLUT_CURSOR_NONE);
    glutEntryFunc(mouse_enter);
    glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
