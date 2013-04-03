/* -*- mode: c++ -*-
 *
 * Demos/GlutExample/randomTargets.cpp --
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

#include "randomTargets.h"

Target::Target()
{
	selected = false;
}

void Target::setDimensions(double x, double y, double z, double size)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->size = size;
}

void Target::display()
{
	glPushMatrix();
		glTranslatef(x,y,z);
		glColor3f(0,1,0);
		glutSolidCube(size);
		glColor3f(0,0,0);
		glutWireCube(size*1.01);
	glPopMatrix();
}

Target::~Target()
{

}

RandomTargets::RandomTargets()
{
#ifdef WIN32
	LARGE_INTEGER startT;
	QueryPerformanceCounter(&startT);
	srand((unsigned int)startT.QuadPart);
#else
	struct timeval stamp ;
    gettimeofday(&stamp, 0) ;
	srand((unsigned int)stamp.tv_usec*1000000LL);
#endif
}

void RandomTargets::Init(int targetNumber, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ,
												  double sizeMin, double maxSize)
{
	Target T;
	double x, y, z, size;
	this->targetNumber = targetNumber;
	this->MinX = MinX;
	this->MaxX = MaxX;
	this->MinY = MinY;
	this->MaxY = MaxY;
	this->MinZ = MinZ;
	this->MaxZ = MaxZ;
	this->sizeMin = sizeMin;
	this->maxSize = maxSize;
	for (int i=1; i<= targetNumber; i++)
	{
		size = RandomNumberBetweenAandB(sizeMin, maxSize);
		x = RandomNumberBetweenAandB(MinX+size/2.0, MaxX-size/2.0);
		y = RandomNumberBetweenAandB(MinY+size/2.0, MaxY-size/2.0);
		z = RandomNumberBetweenAandB(MinZ+size/2.0, MaxZ-size/2.0);
		T.setDimensions(x, y, z, size);
		TargetTab.push_back(T);
	}
}

double RandomTargets::RandomNumberBetweenAandB(double A, double B)
{
	//Sleep(3);
	return  A + ((double)rand() / ((double)RAND_MAX + 1) * (B-A));
}

bool RandomTargets::HitTarget(double PointerX, double PointerY, double PointerZ, bool click)
{
	bool found = false;
	if (click)
		for(itT = TargetTab.begin(); itT < TargetTab.end(); itT++)
		{
			if ((PointerX - (*itT).x)*(PointerX - (*itT).x)+(PointerY - (*itT).y)*(PointerY - (*itT).y) +
				(PointerZ - (*itT).z)*(PointerZ - (*itT).z)<=
				((*itT).size) * ((*itT).size))
			{
				if (!(*itT).selected) found = true; else found = false;
				(*itT).selected = true;
			}
		}
	return found;
}

void RandomTargets::DisplayTargets()
{
	for(itT = TargetTab.begin(); itT < TargetTab.end(); itT++)
	{
		if (!(*itT).selected) (*itT).display();
	}
}

void RandomTargets::ReStart()
{
	TargetTab.erase(TargetTab.begin(), TargetTab.end());
	Init(targetNumber, MinX, MaxX, MinY, MaxY, MinZ, MaxZ, sizeMin, maxSize);
}

bool RandomTargets::AllTargetsSelected()
{
	int nbTargetsSelected = 0;
	for(itT = TargetTab.begin(); itT < TargetTab.end(); itT++)
	{
		if ((*itT).selected)	nbTargetsSelected++;
	}

	return nbTargetsSelected == targetNumber;
}

RandomTargets::~RandomTargets()
{

}
