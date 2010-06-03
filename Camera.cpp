// Camera.cpp
//
// Function implementation of Camera.h
//

#include <iostream>
using namespace std;
#include "Camera.h"
#include "constants.h"
#include <math.h>

#include "Object.h"

//------------------------------------------------------------ CONSTRUCTORS

Camera::Camera()
{
	ncameras=3;
	
	cameraMode = 1;
	
	posit=0;
	
 	xpos = 5; 
	ypos = 7;
	zpos = 5;
	xrot = -45;
	yrot = 45,
		
	xrot2 = 45;
	yrot2 = 20;
	zoom = 5;
}


//------------------------------------------------------------ DESTRUCTORS

Camera::~Camera() { }

//------------------------------------------------------------ GETTERS & SETTERS

char* Camera::getMode () const
{
	switch( cameraMode )
	{
		case 0:	return "Cinematic";
		case 1: return "Manual";
		case 2: return "Ball centered";
		default: return "Unknown";
	}
}

void Camera::setCameraMode (int mode)
{
	if( cameraMode < ncameras )
		cameraMode = mode;
	else
		cout << "error: CameraMode " << mode << "doesnt exists." << endl;
}

int Camera::getCameraMode () const
{
	return cameraMode;
}

void Camera::action1 (int movex, int movey)
{
	switch( cameraMode )
	{
		case 0:
		
			break;
		case 1:
			yrot = yrot + movey / 5.f;
			xrot = xrot + movex / 5.f;	
			break;		
		case 2:
			yrot2 = yrot2 + movey / 5.f;
			xrot2 = xrot2 + movex / 5.f;	
			break;
	}	
}

void Camera::action2 (int movex, int movey)
{
	switch( cameraMode )
	{
		case 0:
		
			break;
		case 1:
			float xrotrad, yrotrad;
			yrotrad = (xrot / 180 * M_PI);
			xrotrad = (yrot / 180 * M_PI);
			xpos += -movey/10.0 * float(sin(yrotrad)) ;
			zpos -= -movey/10.0 * float(cos(yrotrad)) ;
			ypos -= -movey/10.0 * float(sin(xrotrad)) ;		
			break;
		case 2:
			zoom += movey / 5.f;
			if(zoom<1) zoom=1;		// do not let reverse the camera
			break;
	}
}


void Camera::apply(Object* object)
{
	switch( cameraMode )
	{
		case 0:
			posit++;
			// cinematic camera
			gluLookAt( cos(posit/100)*5, 7 , sin(posit/100)*5,
					   object->getPosX(), object->getPosY(), object->getPosZ(), /*ball position*/
					   0,1,0);
			break;
		case 1:
			// controlled movement
			glRotatef(yrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
			glRotatef(xrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
			glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
			break;
		
		case 2:
			// ball centered camera
			float orig[3] = { object->getPosX(), object->getPosY(), object->getPosZ() }; //ball positions
			
			gluLookAt( orig[0] + sin(yrot2*0.05)*sin(xrot2*0.05)*zoom, orig[1] + cos(yrot2*0.05)*zoom, orig[2] + cos(xrot2*0.05)*sin(yrot2*0.05)*zoom,
					   orig[0], orig[1], orig[2],
					   0,1,0);
			break;
	}
	
}

void Camera::nextCameraMode()
{
	setCameraMode( (getCameraMode()+1)%ncameras );
}