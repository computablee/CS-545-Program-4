const char* info = ""
"Phillip Lane\n"
"CS 545 01\n"
"10/27/2020\n"
"Dr. Hauenstein\n"
"\n"
"Software used:\n"
"Windows 10 Pro\n"
"Visual Studio 2017 (v141)\n"
"Windows SDK Toolkit 10.0.18362.0\n"
"\n"
"Hardware used:\n"
"2x Opteron 6380\n"
"64GB DDR3\n"
"Nvidia Quadro K4200\n"
"\n"
"This program demonstrates lighting effects on a teapot.\n"
"\n"
"Controls:\n"
" UP - move light up\n"
" DOWN - move light down\n"
" LEFT - move light left\n"
" RIGHT - move light right\n"
" , - move light towards user\n"
" . - move light away from user\n"
" d - activate diffuse lighting\n"
" s - activate specular lighting\n"
" 2 - activate second light (switches all controls to second light, force enable diffuse and specular lighting)\n"
" + - increase shiney\n"
" - - decrease shiney\n"
" esc - Quit\n"
"\n";

//Just some libraries we're gonna be using
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

//light coordinates
float lightCoords[2][4] = { /*first*/ { 0.0f, 0.0f, 0.0f, 0.0f }, /*second*/ { 0.0f, 0.0f, 0.0f, 0.0f } };
//light values
float lightVal[2][4] = { /*false*/ { 0.0f, 0.0f, 0.0f, 0.0f }, /*true*/ { 1.0f, 1.0f, 1.0f, 1.0f }};
//shiney
float shineylol[1] = { 50.0f };
//whether or not we have two lights
int twoLights = 0;
//whether or not we have diffuse
int diffuse = 0;
//whether or not we have specular
int specular = 0;

// Initialization routine.
void setup(void)
{
	//set clear color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Material property vectors.
	float matAmbAndDif[] = { 0.9f, 0.2f, 0.2f, 1.0f };
	float matSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Material properties of teapot.
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shineylol);

	//enable light 0
	glEnable(GL_LIGHT0);
}

// Drawing routine.
void drawScene(void)
{
	//clear colors
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set light coords and material property of shiney
	glLightfv(GL_LIGHT0, GL_POSITION, lightCoords[0]);
	if (twoLights) glLightfv(GL_LIGHT1, GL_POSITION, lightCoords[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, shineylol);

	//look at teapot
	glLoadIdentity();
	gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//set light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightVal[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightVal[diffuse]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightVal[specular]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightVal[0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightVal[diffuse]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightVal[specular]);

	//set ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float[4]) { 0.2f, 0.2f, 0.2f, 1.0f });

	//enable second light if we need to
	if (twoLights) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

	//draw the teapot
	glFrontFace(GL_CW);
	glutSolidTeapot(1.0);
	glFrontFace(GL_CCW);
	glPopMatrix();

	//swap buffers
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
		//if comma, bring light towards user
	case ',':
		lightCoords[twoLights][2] += 0.5f;
		break;
		//if period, bring light away from user
	case '.':
		lightCoords[twoLights][2] -= 0.5f;
		break;
		//if plus, increase shiney
	case '+':
		shineylol[0] += 5;
		break;
		//if minus, decrease shiney
	case '-':
		shineylol[0] -= shineylol[0] > 0 ? 5 : 0;
		break;
	case '2':
		//if two, enable two lights
		twoLights = 1 - twoLights;
		diffuse = 1;
		specular = 1;
		break;
		//if s, toggle specular
	case 's':
		if (!twoLights) specular = 1 - specular;
		break;
		//if d, toggle diffuse
	case 'd':
		if (!twoLights) diffuse = 1 - diffuse;
		break;
		//if escape, exit
	case 27:
		exit(0);
	default:
		break;
	}

	//redraw
	glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	//move light based on arrow keys
	if (key == GLUT_KEY_UP) lightCoords[twoLights][1] += 0.5f;
	if (key == GLUT_KEY_DOWN) lightCoords[twoLights][1] -= 0.5f;
	if (key == GLUT_KEY_LEFT) lightCoords[twoLights][0] -= 0.5f;
	if (key == GLUT_KEY_RIGHT) lightCoords[twoLights][0] += 0.5f;

	//redraw
	glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv)
{
	printf("%s", info);
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