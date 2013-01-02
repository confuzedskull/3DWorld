#include <stdlib.h>
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red=1.0f, blue=1.0f, green=1.0f;
float zoom=-10.0f;
float panx=0.0f, pany=0.0f, jump=0.0f;
float x_rotation=0.0f, y_rotation=0.0f;

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

	glRotatef(y_rotation, 0.0f, 1.0f, 0.0f);
	glRotatef(x_rotation, 1.0f, 0.0f, 0.0f);
    glTranslatef(panx, pany, zoom);
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

	glBegin(GL_QUADS);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f( -1.0f, 1.0f, -1.0f);
		glVertex3f( -1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
    glColor3f(1.0f,0.5f,0.0f);
		glVertex3f(1.0f, -1.0f,1.0f);
		glVertex3f( -1.0f, -1.0f, 1.0f);
		glVertex3f( -1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f( -1.0f, 1.0f, 1.0f);
		glVertex3f( -1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f( -1.0f, -1.0f, -1.0f);
		glVertex3f( -1.0f, 1.0f, -1.0f);
		glVertex3f( 1.0f, 1.0f, -1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f( -1.0f, 1.0f, -1.0f);
		glVertex3f( -1.0f, -1.0f, -1.0f);
		glVertex3f( -1.0f, -1.0f, 1.0f);
    glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f( 1.0f, 1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
    glTranslatef(panx, pany, 0.0f);
    glLoadIdentity();
	glEnd();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 'w')
        zoom=zoom+0.1;

    if (key == 'a')
        panx=panx + 0.1;

    if (key == 'd')
        panx=panx-0.1;

    if (key == 's')
        zoom=zoom-0.1;

    if (key == 32)
        jump=jump+0.1;


	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_LEFT :
                y_rotation-=1.0;
                break;
		case GLUT_KEY_UP :
				x_rotation -=1.0;
                break;
		case GLUT_KEY_RIGHT :
				y_rotation+=1.0;                                         ;
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
