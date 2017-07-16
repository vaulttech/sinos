#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <map>
#include <string.h>

#include "lib/glm.h"
#include "lib/imageloader.h"
#include "lib/Texture.h"

#include "Object.h"
#include "ObjectModel.h"
#include "ObjectBall.h"
#include "ObjectStick.h"
#include "Camera.h"
#include "LightInfo.h"
#include "Level.h"
#include "Game.h"
#include "constants.h"
#include "utils.h"

// Objects
map<string,Object*> objects;
vector<LightInfo*>  theLights;

// Texture files
static Texture tigerTex, woodTex, tableTex, rockTex, starsTex, ballTex[N_BALL_TEX], stickTex;

// Mother class
Game game;

// mouse-keyboard
static int      xold, yold;
static int      left_click = GLUT_UP;
static int      right_click = GLUT_UP;
static int      middle_click = GLUT_UP;
static bool     heldCtrl = false;
static bool     heldShift = false;
static int      timeBase = 0;
static int      timeAct; 
static bool     biggerViewport = false;
static bool     hardMode = false;

// Window
int width, height;
bool isFullscreen=false;



//--------------------------------------------------------------------//
//----------------------------- OPENGL -------------------------------//
//--------------------------------------------------------------------//

void initObjects ()
{
	static ObjectModel tableMiddle("obj/pooltable_middle.obj");
	static ObjectModel tableBottom("obj/pooltable_bottom.obj");
	static ObjectModel tableTop("obj/pooltable_table.obj");
	static ObjectModel tableFrame("obj/pooltable_frame.obj");
	static ObjectModel tableFrameBound("obj/pooltable_frame_bound.obj");
	glmFacetNormals(tableFrameBound.getModelPointer());
	glmVertexNormals(tableFrameBound.getModelPointer(), -90.0);
	static ObjectModel scenario("obj/crypt.obj");
	static ObjectModel globe("obj/globe.obj");
	//static ObjectModel light("obj/light1.obj");
	/*static ObjectModel tableStruct2("obj/pooltable_struct.obj");
	  static ObjectModel tableTop2("obj/pooltable_table.obj");
	  static ObjectModel tableStruct3("obj/pooltable_struct.obj");
	  static ObjectModel tableTop3("obj/pooltable_table.obj");
	  static ObjectModel tableStruct4("obj/pooltable_struct.obj");
	  static ObjectModel tableTop4("obj/pooltable_table.obj");*/

	// crypt scenario
	scenario.setPos(0,-30,0);
	scenario.setSize(250,350,250);
	scenario.setTexture(&rockTex);
	scenario.material.setDiffuse( RGB(265)*0.4, RGB(234)*0.4, RGB(186)*0.4 );
	objects["crypt"] = &scenario;

	// tables material setup
	Material tableStructMat;
	tableStructMat.setDiffuse(0.6 *1.5, 0.18 *1.5, 0.14 *1.5);
	tableStructMat.setSpecular(0.3,0.3,0.3);
	tableStructMat.setShininess(120);
	Material tableTopMat;
	tableTopMat.setDiffuse(0.078 , 0.66 , 0.078 );
	tableTopMat.setSpecular(0.1,0.1,0.1);
	tableTopMat.setShininess(40);

	// main table 
	//materials
	tableBottom.setMaterial(tableStructMat);
	tableMiddle.setMaterial(tableStructMat);
	tableTop.setMaterial(tableTopMat);
	tableFrame.setMaterial(tableTopMat);
	//tableFrame.material.setEmission(0.1,0.1,0.1);
	//textures
	tableMiddle.setTexture(&woodTex);
	tableBottom.setTexture(&woodTex);
	tableTop.setTexture(&tableTex);
	tableFrame.setTexture(&tableTex);
	//scaling
	tableBottom.setSize(100,100,100); 
	tableMiddle.setSize(100,100,100); 
	tableTop.setSize(100,100,100);
	tableFrame.setSize(100,100,100);
	//pipelining
	objects["tableTop"] = &tableTop;
	objects["tableBottom"] = &tableBottom;
	objects["tableMiddle"] = &tableMiddle;
#ifdef SHOW_TABLE_FRAME
	objects["tableFrame"] = &tableFrame;
	objects["tableFrameBound"] = &tableFrameBound;
#endif

	// table2
	/*tableStructMat.setDiffuse(0.25,0.09,0.07);
	  tableStructMat.setSpecular(0,0,0);
	  tableTopMat.setDiffuse(0.078 *0.5, 0.66 *0.5, 0.078 *0.5);	
	  tableStruct2.setMaterial(tableStructMat);
	  tableTop2.setMaterial(tableTopMat);
	  tableStruct2.setSize(100,100,100);
	  tableTop2.setSize(100,100,100);
	  tableStruct2.setPos(0,0,-300);
	  tableTop2.setPos(0,0,-300);
	// table3
	tableStruct3.setMaterial(tableStructMat);
	tableTop3.setMaterial(tableTopMat);
	tableStruct3.setSize(100,100,100);
	tableTop3.setSize(100,100,100);
	tableStruct3.setPos(0,0,300);
	tableTop3.setPos(0,0,300);
	// table4
	tableStruct4.setMaterial(tableStructMat);
	tableTop4.setMaterial(tableTopMat);
	tableStruct4.setSize(100,100,100);
	tableTop4.setSize(100,100,100);
	tableStruct4.setPos(0,0,600);
	tableTop4.setPos(0,0,600);
	objects["tableStruct2"] = &tableStruct2;
	objects["tableTop2"] = &tableTop2;
	objects["tableStruct3"] = &tableStruct3;
	objects["tableTop3"] = &tableTop3;
	objects["tableStruct4"] = &tableStruct4;
	objects["tableTop4"] = &tableTop4;*/

	// ceiling lamp
	/*light.setPos(0,120,0);
	  light.setSize(50,50,50);
	  light.material.setDiffuse(0.5,0.5,0.5);
	  light.material.setSpecular(1,1,1);
	  light.material.setShininess(120);
	//light.material.setEmission(RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4);
	objects["lamp"] = &light;*/


	// infinite scenario globe
	globe.setSize(500,500,500);
	globe.material.setEmission(1,1,1);
	globe.setTexture(&starsTex);
	objects["globe"] = &globe;
}

void perspectiveViewport( int width, int height ) {
	glMatrixMode (GL_PROJECTION); //set the matrix to projection
	glLoadIdentity ();

	glViewport (0, 0, (GLsizei)width, (GLfloat)height);
	glScissor(0, 0, width, height);

	gluPerspective (60, (GLfloat)width / (GLfloat)height, 1, 10000.0);
	//glOrtho(-65, 65, -35, 35, 5, 500);

	glMatrixMode(GL_MODELVIEW);

	for(int i=0;i<game.level->balls.size();i++)
		game.level->balls[i].updateRotateMatrix();

	glLoadIdentity ();

	// IMPORTANT: don't change the order of these calls
	game.drawOsd();
	game.level->camera->apply();
	game.level->lights();
	game.level->drawObjects();
	game.level->castShadows();
}	

void orthoViewport( int width, int height ) {
	/*if( biggerViewport ) {
	  glViewport( 0.60*width, 0.48*height, width/2, height/2 );
	  glScissor(  0.60*width, 0.48*height, width/2, height/2 );
	  }
	  else {
	  glViewport( 0.75*width, 0.65*height, width/3, height/3 );
	  glScissor(  0.75*width, 0.65*height, width/3, height/3 );
	  }*/
	if( biggerViewport ) {
		glViewport( 0.70*width, 0.70*height, width/3, height/3 );
		glScissor(  0.70*width, 0.70*height, width/3, height/3 );
	}
	else {
		glViewport( 0.75*width, 0.75*height, width/4, height/4 );
		glScissor(  0.75*width, 0.75*height, width/4, height/4 );
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-75,75, -55, 55, 5, 500);
	glOrtho(-65, 65, -35, 35, 5, 500);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	//glRotatef(90, 0,0,1);

	// IMPORTANT: don't change the order of these calls
	game.level->camera2->apply();
	game.level->lights();
	game.level->drawObjects_partial();
}	

void reshape(int w, int h) {

	width = w;
	height = h;
}

#define ACCUM_FRAMES 1

void display () {

	static int draws=0;

	// POG to reduce display rate and priorize variables updating
	static int displayController=0;

	game.updateState();

	displayController += 1;
	if( displayController==UPDATE_PRIORITY_FACTOR )
	{
		displayController=0;

		glEnable(GL_SCISSOR_TEST);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		orthoViewport(width,height);
		perspectiveViewport(width,height);

		glAccum(GL_ACCUM, 1./ACCUM_FRAMES);

		draws += 1;
		if(draws%ACCUM_FRAMES == 0) {

			//glAccum(GL_RETURN, 1.0);
			glutSwapBuffers();
			glClear(GL_ACCUM_BUFFER_BIT);
			game.frameCounter++;
		}

		glDisable(GL_SCISSOR_TEST);
	}
}

void initLights () {

	// ambient light
	GLfloat ambientColor[] = {0.09, 0.05, 0.05, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);


	// position light (sun)
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat diffuseLight[] = { 0.6f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00005);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 160);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

	// spotlight
	GLfloat lampColor[] = {RGB(252) *0.4, RGB(234) *0.4, RGB(186) *0.4, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,   0.0001);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION,0.0001);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);

	// spotlight 2
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 15.0);
	// spotlight 3
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 15.0);
	// spotlight 4
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lampColor);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.001);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.005);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 90);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 15.0);

	// directional light
	GLfloat dirColor[] = {0.5, 0.5, 0.5, 1.0f};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dirColor);


	/*
	// Creating LightInfo vector to the level class
	//
	// THERE IS SOMETHING EXTREMELY WRONG IN THIS PART OF THE CODE.
	// TRY PRINTING THE VALUE OF THE VARIABLES INSIDE THE LIGHTINFO
	// INSTANCES ON THE VECTOR AND YOU'RE GONNA SEE VERY STRANGE
	// VALUES INSTEAD OF THOSE YOU'VE JUST PUT THERE D=
	//
	// FOR NOW, THE LEVEL CLASS IS USING THE LIGHTS() FUNCTION INSTEAD
	// OF THESE LIGHTINFO INSTANCES.

	// position light (sun)
	//GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, position);    
	//GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	LightInfo light0(true, position, true, spot_direction, GL_LIGHT0);
	theLights.push_back(&light0);

	// spotlight
	//GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	LightInfo light1(true, light1_position, true, spot_direction, GL_LIGHT1);
	//theLights.push_back(&light1);

	// spotlight 2
	//GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	//glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);

	GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	LightInfo light3(true, light2_position, true, spot_direction, GL_LIGHT3);
	//theLights.push_back(&light3);

	// spotlight 3
	//GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	//glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	//glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);

	GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	LightInfo light4(true, light3_position, true, spot_direction, GL_LIGHT4);
	//theLights.push_back(&light4);

	// spotlight 4
	//GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	//glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	//glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);

	GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	LightInfo light5(true, light4_position, true, spot_direction, GL_LIGHT5);
	//theLights.push_back(&light5);

	// directional light
	//GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT2, GL_POSITION, direction);

	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	LightInfo light2(true, direction, false, direction, GL_LIGHT2);
	//theLights.push_back(&light2);
	 */
}

void initGL()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//normalizes all normals
	glEnable(GL_NORMALIZE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel (GL_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	initLights();
	glEnable (GL_LIGHT0); // sun
	glEnable (GL_LIGHT1); // spotlight
	//glEnable (GL_LIGHT2); // directional
	glEnable (GL_LIGHT3); // extra spotlight1
	glEnable (GL_LIGHT4); // extra spotlight2
	glEnable (GL_LIGHT5); // extra spotlight3
}

void initWorld()
{    
	cout << "Loading textures..."; 
	loadTexture(&woodTex, "textures/wood.tga");
	loadTexture(&tableTex, "textures/table.tga", true);
	loadTexture(&rockTex, "textures/rock.tga");
	loadTexture(&starsTex, "textures/stars3.tga");
	loadTexture(&stickTex, "textures/stick.tga");
	for(int i=0; i<N_BALL_TEX; i++) {
		char path[50];
		sprintf(path,"textures/Ball%i.tga",i);
		loadTexture(&ballTex[i], path);
	}
	cout << "Done.\n"; 

	cout << "Initializing objects...";
	static Camera camera(1),  //main camera
		      camera2(2); //viewport camera

	static Level level(&objects, &theLights, &camera, &camera2, ballTex, stickTex);
	game.setLevel(&level);

	initObjects();
	cout << "Done.\n";    

	game.updateOsd();
}

//--------------------------- KEYBOARD ---------------------------//
void keyboardFunc (unsigned char key, int x, int y) {

	if ( key==K_SPACEBAR) {
		biggerViewport = !biggerViewport;
	}

	if( game.hasEnded ) {
		if( key=='y' ) {
			game.hasEnded = false;
			game.restartGame();
		}

		if( key=='n' ) {
			exit(0);
		}
	}

	if( key == '1' ) {
		game.updateOsd();
		game.level->camera->setMode( 0 );
	}
	if( key == '2' ) {
		game.updateOsd();
		game.level->camera->setCenter( &(game.level->balls[0]) );
		game.level->camera->setRotX2( game.level->stick.rot[1] );
		game.level->camera->setMode( 1 );
	}
	if( key == '3' ) {
		game.updateOsd();
		game.level->camera->setMode( 2 );
	}

	if ( key==K_ESC ) {
		exit(0);
	}
}

void specialFunc(int key, int x, int y)
{
	/*if( key == GLUT_KEY_F11 ) {
	  static bool fullscreen = false;
	  if( !fullscreen )
		glutEnterGameMode();
	  else
		glutLeaveGameMode();
	  fullscreen = !fullscreen;
	  }*/

	if( key == GLUT_KEY_F12 )
		hardMode = !hardMode;

	int var=2;
	/*
	   if( key == GLUT_KEY_LEFT ) {
	//game.level->balls[0].setPosX( game.level->balls[0].getPosX()-1 );
	game.level->balls[0].moveVector[0] += -var;
	}

	if( key == GLUT_KEY_RIGHT ) {
	//game.level->balls[0].setPosX( game.level->balls[0].getPosX()+1 );
	game.level->balls[0].moveVector[0] += var;
	}

	if( key == GLUT_KEY_DOWN ) {
	//game.level->balls[0].setPosZ( game.level->balls[0].getPosZ()+1 );
	game.level->balls[0].moveVector[2] += var;
	}

	if( key == GLUT_KEY_UP ) {
	//game.level->balls[0].setPosZ( game.level->balls[0].getPosZ()-1 );
	game.level->balls[0].moveVector[2] += -var;
	}*/
}

//--------------------------- MOUSE ---------------------------//
void mouseFunc(int button, int state, int x, int y) {
	/* This function only updates click states and positions */

	if( button == GLUT_LEFT_BUTTON )
		left_click = state;
	if( button == GLUT_RIGHT_BUTTON )
		right_click = state;
	if( button == GLUT_WHEEL_MIDDLE )
		middle_click = state;
	if( button == GLUT_WHEEL_DOWN )
		game.level->camera->action2(0,1);
	if( button == GLUT_WHEEL_UP )
		game.level->camera->action2(0,-1);

	xold = x;
	yold = y;

	heldCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
	heldShift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
}

void mouseMotionFunc(int x, int y) {

	double sf;
	if(heldShift)
		sf = 20.;
	else
		sf = 5.;

	if( left_click == GLUT_DOWN && right_click == GLUT_DOWN )
		game.level->camera->action2(0,(y-yold)/sf);
	else if ( left_click == GLUT_DOWN ) {
		game.level->stick.rotate( (x-xold)/sf );
		if( game.level->camera->getMode() == 1 )
			game.level->camera->action1( (x-xold)/sf,0);
	}
	else if ( right_click == GLUT_DOWN ) {
		if( game.hasControl ) {
			if( hardMode ) {
				game.level->camera->action1( (x-xold)/(sf*2),0);
				game.level->stick.rotate( (x-xold)/(sf*2) );
			}

			game.level->stick.changePower( (y-yold)/sf );
			if( game.level->stick.getAttackStrenght() < game.level->balls[0].getRadius() ) {
				game.attack( sf*(yold-y) );
			}
		}
	}

	if ( middle_click ==GLUT_DOWN  ) {
		if( game.hasControl )
			if( game.level->camera->getMode() == 1 )
				game.level->camera->action1( 0,(y-yold)/sf);
			else
				game.level->camera->action1((x-xold)/sf, (y-yold)/sf);

		else {
			if( game.level->camera->getMode() == 1 )
			{
				game.level->camera->action1( (x-xold)/sf, (y-yold)/sf);
				game.level->stick.rotate( (x-xold)/sf );
			}
			else
				game.level->camera->action1( (x-xold)/sf, (y-yold)/sf);

		}

	}

	xold = x;
	yold = y;
}

void updateFPS(int value) {
	game.fps = game.frameCounter;
	game.frameCounter = 0;
	game.updateOsd();

	glutTimerFunc(1000/*1sec*/, updateFPS, 0);
}

void updateState(int value) {
	game.updateState();
	glutTimerFunc(1000/STATEUPDATES_PER_SEC, updateState, 0);
}

void displayCaller(int value) {
	display();
	glutTimerFunc(1000/STATEUPDATES_PER_SEC, displayCaller, 0);
}

void endTheGame(int value)
{
	game.currentPlayer = 0;
	game.resetPoints();
	game.hasControl = true;

	game.level->endGame();
}

void parseOptions(int argc, char **argv)
{
	char ch;

	while((ch = getopt(argc, argv, "fh")) != EOF) {

		switch(ch) {
			case 'h':
				// Be sure that this print is updated with all options from this 'switch'.
				printf("Command line options:\n");
				printf(" -f\t\t 1920x1080 Full Screen.\n");
				break;

			case 'f':
				isFullscreen = true;
				break;
		}
	}
}

int main (int argc, char **argv) {
	cout << "Initializing...\n";

	parseOptions(argc, argv);

	glutInit (&argc, argv);

	//set the display to Double buffer, with depth and stencil buffers
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );

	if( isFullscreen ) {
		//Full Screen Mode (adjust resolution for your full resolution value)
		glutGameModeString("1920x1080:16@60");
		glutEnterGameMode();
	}
	else {
		glutInitWindowSize (1280, 720);

		//set the position of the window
		glutInitWindowPosition (0, 100);

		glutCreateWindow ("SiNoS := SiNoS is Not Sinuca");
	}

	initGL();
	initWorld();

	glutDisplayFunc (display);
	glutIdleFunc (display);

	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutTimerFunc(0, updateFPS, 0);    
	glutReshapeFunc(reshape);
	//glutTimerFunc(0, updateState, 0);
	//glutTimerFunc(0, displayCaller, 0);

	glutMainLoop(); 


	return 0;
} 

