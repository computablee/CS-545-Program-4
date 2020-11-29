/* Phillip Lane
 * CS 545 01
 * 10/7/2020
 * Dr. Hauenstein
 *
 * Software used:
 * Windows 10 Pro
 * Visual Studio 2017 (v141)
 * Windows SDK Toolkit 10.0.18362.0
 * 
 * Hardware used:
 * 2x Opteron 6380
 * 64GB DDR3
 * Nvidia Quadro K4200
 *
 * This program is a miniature game written in OpenGL. You move a sphere with WASD into a box, whose position is random on the board.
 * Moving the sphere into the box triggers text to appear saying "You win!" You can also speed up and slow down the animation of the sphere.
 *
 * Controls:
 *   W  - move up
 *   A  - move left
 *   S  - move down
 *   D  - move right
 *   +  - speed up the animation
 *   -  - slow down the animation
 *  esc - Quit
 */

//Just some libraries we're gonna be using
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

float lightCoords[2][4] = { /*first*/ { 0.0f, 0.0f, 0.0f, 0.0f }, /*second*/ { 0.0f, 0.0f, 0.0f, 0.0f } };
float lightVal[2][4] = { /*false*/ { 0.0f, 0.0f, 0.0f, 0.0f }, /*true*/ { 1.0f, 1.0f, 1.0f, 1.0f }};
float shineylol[1] = { 50.0f };
int twoLights = 0;
int diffuse = 0;
int specular = 0;

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Material property vectors.
	float matAmbAndDif[] = { 0.9f, 0.2f, 0.2f, 1.0f };
	float matSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Material properties of ball.
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shineylol);

	glEnable(GL_LIGHT0);
}

// Drawing routine.
void drawScene(void)
{
	printf("%f, %f, %f\n", lightCoords[0][0], lightCoords[0][1], lightCoords[0][2]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightCoords[0]);
	if (twoLights) glLightfv(GL_LIGHT1, GL_POSITION, lightCoords[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, shineylol);

	glLoadIdentity();
	gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightVal[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightVal[diffuse]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightVal[specular]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightVal[0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightVal[diffuse]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightVal[specular]);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float[4]) { 0.2f, 0.2f, 0.2f, 1.0f });

	if (twoLights) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
	
	glFrontFace(GL_CW);
	glutSolidTeapot(1.0);
	glFrontFace(GL_CCW);
	glPopMatrix();

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch(key)
	{
	case ',':
		lightCoords[twoLights][2] += 0.5f;
		break;
	case '.':
		lightCoords[twoLights][2] -= 0.5f;
		break;
	case '+':
		shineylol[0] += 5;
		break;
	case '-':
		shineylol[0] -= shineylol[0] > 0 ? 5 : 0;
		break;
	case '2':
		twoLights = 1 - twoLights;
		diffuse = 1;
		specular = 1;
		break;
	case 's':
		if (!twoLights) specular = 1 - specular;
		break;
	case 'd':
		if (!twoLights) diffuse = 1 - diffuse;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) lightCoords[twoLights][1] += 0.5f;
	if (key == GLUT_KEY_DOWN) lightCoords[twoLights][1] -= 0.5f;
	if (key == GLUT_KEY_LEFT) lightCoords[twoLights][0] -= 0.5f;
	if (key == GLUT_KEY_RIGHT) lightCoords[twoLights][0] += 0.5f;

	glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("shiney lighting :)");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}