// constants.h
//
// General purpose macros and constants.
//

#pragma once


#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_MIDDLE 1
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif


// Types
struct point
{
	float x,y,z;
};

// Macros
#define RGB(x) (float)(x)/(255)
#define VARNAME(x) #x
#define RAD(x) ((x)*(M_PI))/(180.)
#define DEGREES(x) ((x)*(180.))/(M_PI)

// Global constants
#define TABLE_PLANE_Y 28.5
#define NHOLES 6
#define NCAMERAMODES 3
#define OSD_LINES 3
#define BALL_RADIUS (1.)
#define BALL_MAX_RES 80
#define BALL_MIN_RES 5
#define NPLAYERS 2
#define N_BALL_TEX 16

// Global variables
#define STATEUPDATES_PER_SEC (4000.) /* the semantics of this constant is confuse without glutTimer
									  * actually, it works like the game clock. Increase to make the game slower.
									  */
#define UPDATE_PRIORITY_FACTOR 80    /* game will re-render again after N updates
									  * increase and rendering will get laggy.
									  */

#define TIME_STEP (1000.)/STATEUPDATES_PER_SEC
#define BALL_DECELERATION_R (0.7)   // table wall friction
#define BALL_DECELERATION_N (0.5)   // table plane friction
#define BALL_G_VEC (0.1) // gravitational acceleration
#define BALL_MIN_SPEED (0.5)
#define BALL_MAX_SPEED BALL_RADIUS*STATEUPDATES_PER_SEC
#define STICK_MAX_STR 30
#define MAX_BACKTRACK 100
#define BACKTRACK_STEP (0.001)
#define LOD_FACTOR (1500.) // increase value -> increase medium quality of balls
#define SHOW_TABLE_FRAME 1 //comment to hide

// Table boundaries
#ifdef SHOW_TABLE_FRAME
	#define TOPBOUND (26.0)-BALL_RADIUS
	#define BOTBOUND (-21.3)+BALL_RADIUS
	#define LEFTBOUND (-46.4)+BALL_RADIUS
	#define RIGHTBOUND (45.8)-BALL_RADIUS
#else
	#define TOPBOUND 27.2
	#define BOTBOUND -22.3
	#define LEFTBOUND -47.4
	#define RIGHTBOUND 47
#endif

// Holes
//cordinates between holes
const float wallLimits[6][2][2] = {   { {-43.25+BALL_RADIUS  , BOTBOUND}, {-3.8+BALL_RADIUS, BOTBOUND} }, 	 //1-2
									  { {3.6-BALL_RADIUS , BOTBOUND}, { 42.9-BALL_RADIUS , BOTBOUND} },	 //2-3
									  
									  { {-43.25+BALL_RADIUS,  TOPBOUND}, {-3.8+BALL_RADIUS,  TOPBOUND} },   //4-5
									  { {3.6-BALL_RADIUS, TOPBOUND}, { 42.9-BALL_RADIUS  ,TOPBOUND} },    //5-6
									  
									  { {LEFTBOUND , 23.0-BALL_RADIUS}, { LEFTBOUND, -18.0+BALL_RADIUS} }, //1-4
									  { {RIGHTBOUND, 23.0-BALL_RADIUS}, { RIGHTBOUND,-18.0+BALL_RADIUS} },  //3-6
								  };  
								  
// holes centers {x,z,radius}
const float HC[6][3] ={{-48. ,-22.9, 2.7},
				       {-0.3 ,-24.4, 2.},
				  	   { 47.4,-22.6, 2.7},
				 	   {-48. , 27.8, 2.7},
					   {-0.3 , 29.3, 2.},
					   { 47.4, 27.5, 2.7}};

// Keyboard keys
enum{
	K_ESC		= 27,
	K_SPACEBAR	= 32
};
	

#define CRYPT 0
#define TABLE_TOP 1
#define TABLE_BOTTOM 2
#define TABLE_MIDDLE 3
#define GLOBE 4

#define TABLE_FRAME 0
#define TABLE_FRAME_BOUND 0


