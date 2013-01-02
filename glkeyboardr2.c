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
float red=1.0f, blue=1.0f, green=1.0f;
float panx=0.0f, pany=0.0f;
float xtrans=0.0f, ytrans=0.0f, ztrans=-10.0f, jump=0.0f;
float x_rotation=0.0f, y_rotation=0.0f;

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos=10.0f;

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen
// angle for rotating triangle
float angle = 0.0f;

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
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	/*gluLookAt(0.0f, jump, zoom,
			panx, 0.0f,  0.0f,
			0.0f, 1.0f, 0.0f);*/
    GLfloat xtrans = -xpos;
    GLfloat ztrans =-zpos;
    GLfloat ytrans = -walkbias-0.25f;
    GLfloat y_rotation = 360.0f -yrot;

	glRotatef(x_rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(xtrans, ytrans, ztrans);

	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, -2.0f);
		glVertex3f( -2.0f, 2.0f, 2.0f);
		glVertex3f( 2.0f, 2.0f, 2.0f);
    glColor3f(1.0f,0.5f,0.0f);
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
    glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f( 2.0f, 2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, 2.0f);
		glVertex3f( 2.0f, -2.0f, -2.0f);
	glEnd();
//small cube//

	glBegin(GL_QUADS);
	glRotatef(angle, 0.0f,1.0f,0.0f);
angle+=0.1f;
glutPostRedisplay();
	glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, 0.0f, -1.0f);
		glVertex3f( -0.5f, 0.5f, -1.0f);
		glVertex3f( -0.5f, 0.5f, 1.0f);
		glVertex3f( 0.5f, 0.5f, 1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, -0.5f,1.0f);
		glVertex3f( -0.5f, -0.5f, 1.0f);
		glVertex3f( -0.5f, -0.5f, -1.0f);
		glVertex3f( 0.5f, -0.5f, -1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, 0.5f, 1.0f);
		glVertex3f( -0.5f, 0.5f, 1.0f);
		glVertex3f( -0.5f, -0.5f, 1.0f);
		glVertex3f( 0.5f, -0.5f, 1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, -0.5f, -1.0f);
		glVertex3f( -0.5f, -0.5f, -1.0f);
		glVertex3f( -0.5f, 0.5f, -1.0f);
		glVertex3f( 0.5f, 0.5f, -1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(-0.5f, 0.5f, 1.0f);
		glVertex3f( -0.5f, 0.5f, -1.0f);
		glVertex3f( -0.5f, -0.5f, -1.0f);
		glVertex3f( -0.5f, -0.5f, 1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(0.5f, 0.5f, -1.0f);
		glVertex3f( 0.5f, 0.5f, 1.0f);
		glVertex3f( 0.5f, -0.5f, 1.0f);
		glVertex3f( 0.5f, -0.5f, -1.0f);
    glTranslatef(xpos, pany, 0.0f);
    glLoadIdentity();

	glEnd();

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
        xpos=xpos - 0.1;

    if (key == 'd')
        xpos=xpos+0.1;

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
        jump=jump+0.1;


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
	glutCreateWindow("3D Cube");

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
