/* -*- mode: c++ -*-
 *
 * Demos/GlutExample/randomTargets.h --
 *
 * Initial software
 * Authors: Gery Casiez (gery.casiez@lifl.fr)
 * Copyright University Lille 1, Inria
 *
 * https://code.google.com/p/libgametrak/
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#ifndef __RANDOMTARGETS_h__
#define __RANDOMTARGETS_h__

#include <vector>
#ifndef _MSC_VER // Visual Studio C++
#include <sys/time.h>
#endif
#include <time.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h> 
#endif

#ifdef WIN32
#include <Windows.h>
#include <GL/gl.h>
#endif

class Target
{
private:

public:
	double x;
	double y;
	double z;
	double size;
	bool selected;
	Target();
	void setDimensions(double x, double y, double z, double size);
	void display();
	~Target();
};

class RandomTargets
{
private:
	int targetNumber;
	double MinX;
	double MaxX;
	double MinY;
	double MaxY;
	double MinZ;
	double MaxZ;
	double sizeMin;
	double maxSize;
	std::vector<Target> TargetTab;
	std::vector<Target>::iterator itT; // iterator for targets
	double RandomNumberBetweenAandB(double A, double B);

public:
	RandomTargets();
	void Init(int targetNumber, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ,
												  double sizeMin, double maxSize);
	bool HitTarget(double PointerX, double PointerY, double PointerZ, bool click);
	bool AllTargetsSelected();
	void ReStart();
	void DisplayTargets();
	~RandomTargets();
};

#endif
