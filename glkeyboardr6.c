#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// all variables initialized to 1.0, meaning
// the triangle will initially be white
struct object{float x; float y; float z; float resting; int limits; int action; int status;};

float panx=0.0f, pany=0.0f;
float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float x_rotation=0.0f, y_rotation=0.0f;

float color=0.0f;
float togglecube=0.0f;

const float piover180 = 0.0174532925f;
float heading;

char text1[256];
char text2[256];
char text3[256];
char text4[256];

float xpos=0;
float ypos=0;
float zpos=10;
object player;

int standing;
int jumping;
int falling;

float cubeposx=0.0f;
float cubeposy=0.0f;
float cubeposz=0.0f;
object cube;

GLfloat	yrot;				// Y Rotation
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

void renderScene(void) {

    player.x=xpos;
    player.y=ypos;
    player.z=zpos;
    player.action=standing;
    player.limits=1;

    cube.x=cubeposx;
    cube.z=cubeposz;

    float referencex=player.x;
    float referencey=player.y;
    float referencez=player.z;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

    sprintf(text1,"x=%f",player.x);
    //statx=text1;
    sprintf(text2,"y=%f",player.y);
    //staty=text2;
    sprintf(text3,"z=%f",player.z);
    sprintf(text4, "health: %d", player.status);
    //statz=text3;

	// Set the camera
	/*gluLookAt(-xtrans, ytrans, -ztrans,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);*/
    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    //GLfloat ytrans = -walkbias-0.25f;
    GLfloat ytrans = -ypos;
    GLfloat y_rotation = 360.0f -yrot;

	glRotatef(x_rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(xtrans, ytrans, ztrans);
   // glRasterPos3f(referencex, referencey, referencez);
    glPushMatrix();
    gravity(player);
    glScalef(2.0,1.0,2.0);
	glColor3f(0.0f,1.0f,1.0f);
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
    glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(2.0f, 2.0f, 2.0f);
		glVertex3f( -2.0f, 2.0f, 2.0f);
		glVertex3f( -2.0f, -2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, 2.0f);
    glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(2.0f, -2.0f, -2.0f);
		glVertex3f( -2.0f, -2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, -2.0f);
		glVertex3f( 2.0f, 2.0f, -2.0f);
    glColor3f(0.0f,0.0f,1.0f);
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

glRotatef(angle, 0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);

angle+=0.01f;
glutPostRedisplay();
	glColor3f(color,0.0f,1.0f);
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
	ViewOrtho(320,320);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f,0.0,0.0f);
    glBegin(GL_LINES);
        glVertex2f(160, 150);  // left - x negative
        glVertex2f(160, 170);   // right - x positive

        glVertex2f(150,160);
        glVertex2f(170,160);
    glEnd();

glPushMatrix();
    glutPrint (0.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text1, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,20.0f, GLUT_BITMAP_HELVETICA_12, text2, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (0.0f,30.0f, GLUT_BITMAP_HELVETICA_12, text3, 1.0f,1.0f,1.0f, 0.5f);
    glutPrint (200.0f,10.0f, GLUT_BITMAP_HELVETICA_12, text4, 1.0f,1.0f,1.0f, 0.5f);
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
        xpos -= (float)sin(heading*piover180) * 0.05f;
        zpos -= (float)cos(heading*piover180) * 0.05f;
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
        xpos -= (float)cos(heading*piover180) * 0.05f;

        zpos += (float)sin(heading*piover180) * 0.05f;
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
        xpos += (float)cos(heading*piover180) * 0.05f;
        zpos -= (float)sin(heading*piover180) * 0.05f;
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
        xpos += (float)sin(heading*piover180) * 0.05f;
        zpos += (float)cos(heading*piover180) * 0.05f;
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
				x_rotation -=1.0;
                break;
		case GLUT_KEY_RIGHT :
				heading-=1.0f;
				yrot=heading;                                         ;
				break;
        case GLUT_KEY_DOWN :
				x_rotation +=1.0;
				break;
	}
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("3D World");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	// here are the new entries
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
