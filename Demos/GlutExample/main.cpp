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

//#include <windows.h>
//#include <GL/glut.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
 #include <iomanip>

#include "randomtargets.h"

#include <libgametrak/GameTrak.h>

using namespace std;
using namespace gametrak;

RandomTargets RT;

bool perspectiveOrthoPointers = false;

double HandsDistance = 100.0;

Vecteur3D Scale = Vecteur3D(0.03,0.04,0.04);
Vecteur3D Translate = Vecteur3D(0.0,-5.0,0.0);


// For targets
double nb = 10, xmin = -7, xmax = 7, ymin = 0, ymax = 15, zmin = -8, zmax = -2, sizemin = 0.5, sizemax = 1.0;


Vecteur3D gametrakLeftHand;
Vecteur3D gametrakRightHand;

bool debug = false;

GameTrak *gt = 0 ;
TimeStamp::inttime last_time = 0 ;
bool button_pressed = false ;

void
GameTrakCallback(void *context, 
             TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button)  {

    gametrakLeftHand.x=leftx;
    gametrakLeftHand.y=lefty;
    gametrakLeftHand.z=leftz;

    gametrakRightHand.x=rightx;
    gametrakRightHand.y=righty;
    gametrakRightHand.z=rightz;

    if (debug) {
        double freq = 1/((timestamp - last_time) * 1.0E-9);
        std::cout << timestamp << " ns, " 
            << std::setw(7) << freq << " Hz, "
            << "Left: (" << std::setw(3) << leftx << ", " << std::setw(3) << lefty << ", " << std::setw(3) << leftz << ") mm"
            << ", Right: (" << std::setw(3) << rightx << ", " << std::setw(3) << rightx << ", " << std::setw(3) << rightz << ") mm, "
            << "button: " << button << std::endl ;
        last_time = timestamp;
    }
}

void initGL()
{	
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


    RT.Init(nb,
            xmin,xmax,
            ymin,ymax,
            zmin,zmax,
            sizemin,sizemax);

}

void GLDisplayString(char* s, float x, float y, GLfloat scale)
{
    int size = strlen(s);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-400.0,400.0,-300.0,300.0,1.0,-1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glTranslatef(x*800.0-400.0, y*600.0-300.0, 0.0);
    glScalef(scale, scale, 1.0);
    for(int j=0; j<size; j++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN , s[j]);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}

void Grid(float width, float height, float step)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (float x = -width/2.0; x <= width/2.0; x+=step)
    {
        glBegin(GL_QUAD_STRIP);
        for (float z = -height/2.0; z <= height/2.0; z+=step)
        {
            glVertex3f(x,0,z);
            glVertex3f(x+step,0,z);
        }
        glEnd();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    Vecteur3D RightP, LeftP;

    // Compute pos
    RightP = gametrakRightHand;
    LeftP = gametrakLeftHand;

    RightP.x += HandsDistance/2.0;
    LeftP.x -= HandsDistance/2.0;
    RightP.x *= Scale.x;
    RightP.y *= Scale.y;
    RightP.z *= Scale.z;
    LeftP.x *= Scale.x;
    LeftP.y *= Scale.y;
    LeftP.z *= Scale.z;
    RightP += Translate;
    LeftP += Translate;


    if (perspectiveOrthoPointers)
    {
        float DistanceBetweenCameraAndWorldOrigin = 21;
        float DistanceCameraScreen = 15;
        RightP.x *= ((DistanceBetweenCameraAndWorldOrigin-RightP.z) / DistanceBetweenCameraAndWorldOrigin);
        RightP.y *= ((DistanceBetweenCameraAndWorldOrigin-RightP.z) / DistanceBetweenCameraAndWorldOrigin);
        //LeftP.x *= ((DistanceBetweenCameraAndWorldOrigin-LeftP.z) / DistanceCameraScreen);
        //LeftP.y *= ((DistanceBetweenCameraAndWorldOrigin-LeftP.z) / DistanceCameraScreen);
    }


    glPushMatrix();
    glTranslatef(0,-5,-20);
    glColor3f(1,1,1);
    Grid(20,30,0.5);


    RT.DisplayTargets();

    glColor3f(1,0,0);
    glPushMatrix();
    glTranslatef(RightP.x, RightP.y, RightP.z);
    glutSolidSphere(0.3,20,20);
    glColor3f(0,0,0);
    glutWireSphere(0.31,7,7);
    glPopMatrix();

    glColor3f(0,0,1);
    glPushMatrix();
    glTranslatef(LeftP.x, LeftP.y, LeftP.z);
    glutSolidSphere(0.3,20,20);
    glColor3f(0,0,0);
    glutWireSphere(0.31,7,7);
    glPopMatrix();

    glPopMatrix();

    if (RT.AllTargetsSelected()) RT.ReStart();
    if (debug)
    {
        char txt[100];

        // Display
        // sprintf(txt, "%3.0f Hz",freq);
        // glColor3f(1,1,1);
        // GLDisplayString(txt, 0.1,0.9,0.2);

        sprintf(txt,"%3.1f %3.1f %3.1f\r", RightP.x, RightP.y, RightP.z);
         GLDisplayString(txt, 0.1,0.8,0.2);
    }

    //Sleep(20);
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char touch,int x,int y)
{
    switch (touch)
    {
    case 27:
        exit(0);
        break;
    case 'd':
        debug=!debug;
        break;
    case 'r':
        RT.ReStart();
        break;
    case 'p':
        perspectiveOrthoPointers=!perspectiveOrthoPointers;
        break;
    }
}

void idle(){

}


void mouse(int button, int state,int x,int y)
{
    //if (button==GLUT_LEFT_BUTTON)
    //{
    //  if (state==GLUT_DOWN) click = true; else click = false;
    //}

    //if (button==GLUT_RIGHT_BUTTON)
    //{
    //  if (state==GLUT_DOWN) clickRight = true; else clickRight = false;
    //}
}

void reshape(int w,int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-800.0, 800.0, -800.*h/(float)w, 800.0*h/(float)w, -1.0, 1.0);
    gluPerspective(45,w/h,1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE  | GLUT_DEPTH);
    if (true)
    {
        glutInitWindowPosition(200,200);
        glutInitWindowSize(1000,1000*3/4.0);
        //glutInitWindowSize(600,300);
        glutCreateWindow("libgametrak demo");
    }
    else
	//Allow to pass in fullscreen mode
	//if(MessageBox(NULL, "Full screen mode?", "Options", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        char txt[50];
        sprintf(txt, "%dx%d:24@60",glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        //glutGameModeString("1024x768:24@60");	//1024*768, 32bit pixel depth, 85Hz refresh rate
        glutGameModeString(txt);
        glutEnterGameMode();					//start fullscreen game mode
    }

    GameTrak *gt= GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1") ;
    gt->setGameTrakCallback(GameTrakCallback) ;

    initGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
