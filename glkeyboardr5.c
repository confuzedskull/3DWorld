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
struct object{float x; float y; float z; float resting; float limits; int action;};

float panx=0.0f, pany=0.0f;
float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float x_rotation=0.0f, y_rotation=0.0f;

float color=0.0f;
float togglecube=0.0f;

const float piover180 = 0.0174532925f;
float heading;



float xpos;
float ypos;
float zpos=10.0f;
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

    if (distance(object1.x,object1.z,object2.x, object2.z) <= 1)
    return true;
    else
    return false;
}

void gravity(object A)
{
    if (A.y>A.resting)
    {   A.y-=0.01;
        ytrans-=0.01;
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


    if(touching(player,cube)==true)
    {
        if (color==0.0)
        togglecube=1.0f;
        if (color==1)
        togglecube=0.0;
    }
    if(distance(xpos,zpos,cubeposx,cubeposz) > 1)
    color=togglecube;

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

void renderScene(void) {

    player.x=xtrans;
    player.y=ytrans;
    player.z=ztrans;

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

	// Set the camera
/*	gluLookAt(-xtrans, player.y, -ztrans,
			referencex, referencey, referencez,
			0.0f, 1.0f, 0.0f);*/
    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    GLfloat ytrans = -walkbias-0.25f;
    GLfloat y_rotation = 360.0f -yrot;

	glRotatef(x_rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(xtrans, ytrans, ztrans);
    glRasterPos3f(referencex, referencey, referencez);
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
	glutSwapBuffers();
}





void processNormalKeys(unsigned char key, int x, int y) {

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
    if (key == 'a')
    {
        xpos -= (float)cos(heading*piover180) * 0.05;
        zpos += (float)sin(heading*piover180) * 0.05;
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

    if (key == 32 && player.action==standing && player.y!=(player.resting+2))
    {
        if(player.action==standing)
        {
            ytrans+=2.0;
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
