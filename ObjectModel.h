// ObjectModel.h
//
// Every object that uses the glm library to be loaded is an ObjectModel.
// It extends the Object class and has a pointer to a GLMmodel, the str-
// uct declared in glm.h that is used to parse the obj files.
//

#pragma once

#include "lib/glm.h"
#include "Object.h"
#include <string>
#include <iostream>

using namespace std;

class ObjectModel: public Object
{
	private:
		GLMmodel* modelPointer;
	public:
		//---------------------------- CONSTRUCTORS
		ObjectModel();
		ObjectModel(GLMmodel* model);
		ObjectModel(const char* filename);

		//---------------------------- DESTRUCTORS
		~ObjectModel();

		//---------------------------- GETTERS & SETTERS
		GLMmodel* 	getModelPointer() const;
		void		setModelPointer(GLMmodel* model);

		//---------------------------- OTHER METHODS
		void			loadFromFile(const char* file);
		void			calculateNormals();
		virtual void	draw() const;
		void			drawNormals() const;
};

