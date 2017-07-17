#include "Level.h"

#define DRAW_GUIDELINES 0

// Balls


//------------------------------------------------------------ CONSTRUCTORS
Level::Level(	map<int, Object*> *_objects, vector<LightInfo*> *_theLights,
				Camera *_camera, Camera *_camera2,
				Texture _ballTex[], Texture _stickTex)
{	
	objects   = _objects;
	theLights = _theLights; 

	camera  = _camera;
	camera2 = _camera2;
	
    for(int i=0; i<N_BALL_TEX; i++)
        ballTex[i] = _ballTex[i];
    stickTex = _stickTex;

    initBalls( TRIANGULAR );

	camera->setCenter( &balls[0] );
	camera->setRotX2( stick.rot[1] );
	
	stick.setCenter(&balls[0]);
	stick.loadFromFile("obj/taco.obj");	
	stick.material.setDiffuse(RGB(238),RGB(221),RGB(195));
	stick.material.setSpecular(0.3,0.3,0.3);
	stick.material.setShininess(80);
	stick.setTexture(&stickTex);
	stick.setSize(7,7,8);
}

//------------------------------------------------------------ DESTRUCTORS
Level::~Level()	{ }

//------------------------------------------------------------ GETTERS & SETTERS

//------------------------------------------------------------ OTHER METHODS

void Level::initBalls( int mode )
{
    #define nballs 16
	
    // common balls material
    Material ballMaterial;
	ballMaterial.setShininess(128); 
	ballMaterial.setAmbient(0.7, 0.7, 0.7); 
	ballMaterial.setEmission(0.1, 0.1, 0.1); 
	ballMaterial.setDiffuse(0.5, 0.5, 0.5);
	ballMaterial.setSpecular(0.9, 0.9, 0.9);
	
	// set balls vector size
	balls.resize(nballs);
	
	// initializes white ball
	balls[0].setRadius(BALL_RADIUS);
	balls[0].setTexture(&ballTex[0]);
	balls[0].setMaterial(ballMaterial);
	balls[0].setPos(-20, TABLE_PLANE_Y+balls[0].getRadius(), 0);
	
    switch( mode )
    {
        case TRIANGULAR:
            // triangular disposition
        	for( int line=1, i=1; i<nballs; )
        	{
        		for( int j=0; j<line && i<nballs; j++, i++ )
        		{
        			balls[i].setRadius(BALL_RADIUS);
        			balls[i].setTexture(&ballTex[i]);
        			balls[i].setMaterial(ballMaterial);
        			balls[i].setPos((BALL_RADIUS*2.1)*line +20, TABLE_PLANE_Y+balls[i].getRadius(), (BALL_RADIUS*2.1)*j - (line-1) );
        		}
        		line++;
        	}
            break;
    
    	case LINEAR:
            // Linear disposition
        	for( int i=1; i<nballs; i++ )
        	{
        		double r = getRandBetween(0,60),
        			   g = getRandBetween(0,60),
        			   b = getRandBetween(0,60);
        		balls[i].setRadius(BALL_RADIUS);
        		balls[i].setTexture(&ballTex[i]);
        		balls[i].setMaterial(ballMaterial);
        		balls[i].material.setDiffuse(r/100.,g/100.,b/100.);
        		balls[i].setPos((BALL_RADIUS*2.1)*i +20, TABLE_PLANE_Y+balls[i].getRadius(), 30 );
        	}
        	break;
    }    
    
}

void Level::drawGuides()
{
    glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 1);
		
		// Holes delimiters
		for(int i=0; i<NHOLES; i++)
			glCircle3f(HC[i][0],TABLE_PLANE_Y+1,HC[i][1],HC[i][2]);
				
		// Table area
		((ObjectModel*)(*objects)[TABLE_FRAME_BOUND])->drawNormals();
		
		// Holes entrances
		for(int i=0; i<6; i++){  
			glBegin(GL_LINES); 
				glVertex3f(wallLimits[i][0][0], TABLE_PLANE_Y, wallLimits[i][0][1]);
				glVertex3f(wallLimits[i][1][0], TABLE_PLANE_Y, wallLimits[i][1][1]);
			glEnd();
		}
		
		// Balls vectors
		for(int i=0; i<balls.size(); i++)
			balls[i].drawVectors();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
}

void Level::drawObjects () {
	if(DRAW_GUIDELINES) drawGuides();
		
	stick.draw();
	
	// draw all objects
	map<int,Object*>::iterator it;
	for( it = objects->begin(); it != objects->end(); it++ )
		if( it->first != CRYPT)
			it->second->draw();
			
	// draw balls
	for( int i=0; i<balls.size(); i++ ) {
		// Linear variation over Level of Detail
		double res = LOD_FACTOR / getDistance(camera->getPosX(), camera->getPosY(), camera->getPosZ(),
											  balls[i].getPosX(), balls[i].getPosY(), balls[i].getPosZ());
		if( res<BALL_MIN_RES )	res=BALL_MIN_RES; 
		else if( res>BALL_MAX_RES) res=BALL_MAX_RES;
		balls[i].setResolution( (int)res );
		balls[i].draw();
	}
	
	glEnable (GL_LIGHT2);
		(*objects)[CRYPT]->draw();
	glDisable (GL_LIGHT2);

	// draw guideline on Main Viewport
	/*if( !stick.isHidden )
		makeGuideLine( &balls, stick.rot[1] );*/
}

void Level::drawObjects_partial ()
{
	stick.draw();
	if( !stick.isHidden )
		makeGuideLine( &balls, stick.rot[1] );

	cout << "Will draw tableMiddle" << endl;

	fprintf(stderr, "in drawObjects_partial: level->objects: %x\n", objects);
	for (map<int, Object*>::iterator it = objects->begin();
		it != objects->end(); it++) {
		cout << "Key: " << it->first << "," << it->second <<
			"," << (*objects)[it->first] << endl;
	}

	map<int, Object*>::const_iterator pos = objects->find(TABLE_MIDDLE);
	if (pos == objects->end()) {
		fprintf(stderr, "tableMiddle not in objects\n");
	} else {
		fprintf(stderr, "tableMiddle points to %x\n", (*objects)[TABLE_MIDDLE]);
	}

	(*objects)[TABLE_MIDDLE]->draw();
	cout << "Finished drawing tableMiddle" << endl;

	(*objects)[TABLE_BOTTOM]->draw();
	(*objects)[TABLE_TOP]->draw();

	// draw balls
	for( int i=0; i<balls.size(); i++ ) {
		if( !balls[i].hasFallen ) {
			balls[i].setResolution(20);
			balls[i].draw();
		}
	}

	#ifdef SHOW_TABLE_FRAME
		(*objects)[TABLE_FRAME]->draw();
	#endif
	
}

void Level::castShadows() {
/* TO DO: modularize shadowing */
	
	float lightSource[] = { 0, 100, 0, 1 };
	float tablePlane [] = { 0, TABLE_PLANE_Y, 0 };
	float floorPlane [] = { 0, 0, 0 };
	float planeNormal[] = { 0, -1, 0 };

	glDisable(GL_LIGHTING); 					//lights don't affect shadows
	glEnable (GL_BLEND); 						//enable transparency
	glEnable(GL_STENCIL_TEST); 					//enable stencil testing when drawing polygones
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); 	// sets operations with stencil buffer when: 
												// stencil_test = false
												// stencil_test = true && depth_test = false
												// stencil_test = true
												
	glColor4f(0,0,0,0.5); 						//black colored shadow with 50% transparency

	// Cast shadows on the ground
	glPushMatrix();
		glShadowProjection(lightSource,floorPlane,planeNormal); //manipulates the matrix so everything will be projected in the FLOORPLANE
			glClear( GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_EQUAL, 0, 1); 						//will let only one vertex be drawn on each position each time shadow
				(*objects)[TABLE_MIDDLE]->draw();
				(*objects)[TABLE_TOP]->draw();
				stick.draw();
	glPopMatrix();
	
	// Cast shadows on the table
	glPushMatrix();
		glShadowProjection(lightSource,tablePlane,planeNormal); //manipulates the matrix so everything will be projected in the TABLEPLANE
			glClear( GL_STENCIL_BUFFER_BIT);
			glColorMask(false, false, false, false); 			//disables Color Buffer
			glDepthMask(false); 								//disables Depth Buffer
			glStencilFunc(GL_ALWAYS, 1, 1); 					//creates the mask that will define where shadow will be cast
				(*objects)[TABLE_TOP]->draw();
			
			glColorMask(true,true,true,true);
			glDepthMask(true); 
			glStencilFunc(GL_EQUAL, 1, 1); 						//now the shadow is cast only where stencil buffer==1, i.e. the table plane
				stick.draw();
				for(int i=0;i<balls.size();i++)
					if( !balls[i].hasFallen )
						balls[i].draw();
				#ifdef SHOW_TABLE_FRAME
					(*objects)[TABLE_FRAME]->draw();
				#endif
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
}

void Level::lights()
{
	//for (int i = 0; i < theLights->size(); i++)
	//	(*theLights)[i]->apply();
	
		// position light (sun)

	GLfloat position[] = { 0 , 100 , 0 , 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);    
	GLfloat spot_direction[] = { 0.0 , -1.0 , 0.0 , 0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	
	
	 //spotlight
	GLfloat light1_position[] = { 0.0, 100.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	
	 //spotlight 2
	GLfloat light2_position[] = { 0.0, 100.0, -300.0, 1.0 };
	glLightfv(GL_LIGHT3, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	
	 //spotlight 3
	GLfloat light3_position[] = { 0.0, 100.0, 300.0, 1.0 };
	glLightfv(GL_LIGHT4, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction);
	
	
	 //spotlight 4
	GLfloat light4_position[] = { 0.0, 100.0, 600.0, 1.0 };
	glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_direction);
	
	 //directional light
	GLfloat direction[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT2, GL_POSITION, direction);
}

vector<int> Level::updateState(int whichPlayer)
/* returns points the player has done */
{
	int ballSet = (whichPlayer - 1) % NPLAYERS; // 0: 1 - 7
												// 1: 9 - 16
	
	pair<bool,bool> retValue;
	int  enemyBall = 0, playerBall = 0;
	bool hasChanged = false;
	
	int otherPlayerFirstBall = 0;
	
	int points=0;
	
	for( int i=0; i<balls.size(); i++ )
	{
		retValue = balls[i].updateState();
	
		hasChanged = retValue.first || hasChanged;	
		if(retValue.second)
			if (ballSet == 0)
				if( i < 8 && i > 0)
					playerBall++;
				else if ( i > 8 )
					enemyBall++;
			else
				if( i < 8 && i > 0)
					enemyBall++;
				else if ( i > 8 )
					playerBall++;
	}

	if( hasChanged )
	{
		otherPlayerFirstBall = testBallsCollision(ballSet);

		camera->setCenter(&balls[0]);
	}
	
	vector<int> returnValue;
	
	returnValue.push_back(hasChanged);
	returnValue.push_back(otherPlayerFirstBall);
	returnValue.push_back(playerBall);
	returnValue.push_back(enemyBall);
	
	//if(otherPlayerFirstBall > 0)
	//	cout << "otherPlayerFirstBall : " << otherPlayerFirstBall << endl;
		
	return returnValue;
}

int Level::testBallsCollision(int ballSet)
{
	int otherPlayerFirstBall = 0;		// 0: no collisions with the main ball;
										// 1: main ball hit a "friendly" ball;
										// 2: main ball hit an enemy ball
																			
	/* 
	 * Collision tests with table's frame (obj's based detection)
	 * 
	 */
	GLMmodel* model = ((ObjectModel*)(*objects)[TABLE_FRAME_BOUND])->getModelPointer();
	double x2 = (*objects)[TABLE_FRAME_BOUND]->pos[0],
		   y2 = (*objects)[TABLE_FRAME_BOUND]->pos[1],
		   z2 = (*objects)[TABLE_FRAME_BOUND]->pos[2];
	double sx = (*objects)[TABLE_FRAME_BOUND]->size[0],
		   sy = (*objects)[TABLE_FRAME_BOUND]->size[1],
		   sz = (*objects)[TABLE_FRAME_BOUND]->size[2];
	double truex, truey, truez;
	
	
	for(int i=0; i<balls.size(); i++) {
		
		/* 
		 * Collisions with walls
		 * 
		 */
		if( !balls[i].hasFallen && !balls[i].isInField() )
		{
			/* Horizontal wall collision */
			if( balls[i].collidedHWall() ) {
				int rep=0;
				while( !balls[i].isInField() && rep++ < MAX_BACKTRACK )
					balls[i].backTrack(balls[i].moveVector);
							
				balls[i].moveVector[0] = -balls[i].moveVector[0];   //reflection
				balls[i].moveVector[0] *= BALL_DECELERATION_R;      //absortion of energy by the wall will be a percentage of horizontal movement
			}
			else
				/* Verticall wall collision */
				if( balls[i].collidedVWall() ) {
					int rep=0;
					while( !balls[i].isInField() && rep++ < MAX_BACKTRACK )
						balls[i].backTrack(balls[i].moveVector);					
				  
					balls[i].moveVector[2] = -balls[i].moveVector[2];   //reflection
					balls[i].moveVector[2] *= BALL_DECELERATION_R;      //absortion of energy by the wall will be a percentage of vertical movement 
				}
				else
					/* Corner wall collision */
					for(int v=0; v<model->numvertices; v++)
					{
						truex = x2 + sx*model->vertices[3*v + 0];
						truey = y2 + sy*model->vertices[3*v + 1];
						truez = z2 + sz*model->vertices[3*v + 2];
			
						if( getDistance(balls[i].pos[0],balls[i].pos[1],balls[i].pos[2], truex,truey,truez) < BALL_RADIUS )
						{
							while( getDistance(balls[i].pos[0],balls[i].pos[1],balls[i].pos[2],truex,truey,truez) < BALL_RADIUS )
								balls[i].backTrack( balls[i].moveVector );
							
							balls[i].reflectAngle(x2+sx*model->normals[3*v+0], y2+sy*model->normals[3*v+1], z2+sz*model->normals[3*v+2]);
						}
					}			
		
		}
		
		
		/* 
		 * Collisions between balls
		 * 
		 */
		for(int j=0; j<balls.size(); j++)
		
			if(i!=j && !balls[i].hasFallen && !balls[j].hasFallen)
		
				if( getDistance(balls[i],balls[j]) < 2*BALL_RADIUS ) // is distance > sum of their radius
				{
					double impactv[3] = { balls[i].pos[0] - balls[j].pos[0], //impactVector = balls[i] - balls[j]
										 0,
										 balls[i].pos[2] - balls[j].pos[2]};
							
							
					// If the first ball a player's ball hit is an enemy's ball,
					// then the enemy "lose" a ball.			 
					if ( i == 0 )
						if (ballSet == 0)		// if it's the player1's turn
							if (j < 8)
								otherPlayerFirstBall = 1;
							else
								otherPlayerFirstBall = 2;
						else
							if (j > 8)			// if it's the player2's turn
								otherPlayerFirstBall = 1;
							else
								otherPlayerFirstBall = 2;
									
													
					// "backtracking"
					int repetitions=0;
					while( getDistance(balls[i],balls[j]) < 2*BALL_RADIUS // lower limit
						  && repetitions++ < MAX_BACKTRACK ) {			  // upper limit
						balls[i].backTrack( impactv, true );
						balls[j].backTrack( impactv);
					}
					
					float impactAngle = getVectorAngle(impactv);
					
					float impactForce = (balls[i].getSpeed() + balls[j].getSpeed())/2;
					
					balls[i].applyForce( impactForce, impactAngle );
					balls[j].applyForce( impactForce, impactAngle, true );
				}
	}
	//if (otherPlayerFirstBall > 0)
	//	cout << "otherPlayerFirstBall: " << otherPlayerFirstBall << endl;
	return otherPlayerFirstBall;
}

void Level::endGame()
{
    for(int i=0; i<balls.size(); i++)
        balls[i].reset();
        
	initBalls( TRIANGULAR );
	
	stick.setCenter(&balls[0]);
}
