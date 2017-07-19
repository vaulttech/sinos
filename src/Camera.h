// Camera.h
//
// 

#pragma once

#include <iostream>
using namespace std;
#include <string>
#include "lib/glm.h"
#include "Object.h"


#define SPIRAL_MOVEMENT_EQUATION cos(posit/100)*(100000/posit), 10 , sin(posit/100)*(100000/posit)


class Camera
{
private:

	int cameraMode;
	int posit;
	double xpos,  ypos,  zpos,	// variables used in the Manual mode
		   xrot,  yrot,			// 
			
		   xpos2, ypos2, zpos2,	// variables used in the Ball Centered mode
		   xrot2, yrot2, zoom,	//
		   xorig, yorig, zorig,	//
			
		   xpos3, ypos3, zpos3;	// variables used in the Top View mode

public:
	//---------------------------- CONSTRUCTORS
	Camera( int cameraMode=0 );

	//---------------------------- DESTRUCTORS
	~Camera();
	
	//---------------------------- GETTERS & SETTERS

	void			setPosX1(GLfloat newValue);
	void			setPosY1(GLfloat newValue);
	void			setPosZ1(GLfloat newValue);
	void			setCamera( int cameraMode = 0 );		// reset to the default camera values.
	void			setRotX2(GLfloat newValue);
	void			setRotY2(GLfloat newValue);
	void			setMode( int mode );
	void			setCenter( Object* object );
	
	/* Set the position for the "number 2" variables of the camera - which
	 * are related to the "camera 2" position.
	 */
	void			setPos2();
	const char* 	getModeName( int mode=-1 ) const; //name of the current camera mode
	int 			getMode() const;
	double			getPosX() const;
	double			getPosY() const;
	double			getPosZ() const;
	
	//---------------------------- OTHER METHODS
	void 			apply( int forceAMode=-1 ); 					// main method
	void 			action1 ( double movex, double movey);					// usually, rotating the view
	void 			action2 ( double movex, double movey);					// usually, zooming
	
	/* If an object is passed, then it will make difference when using the
	 * "camera mode 1". The camera will center on the object passed, instead
	 * of the default value, which is the center of the main table.
	 */
	void 			nextMode(Object* object=NULL);
};
