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


#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h> 
#endif

#ifdef WIN32
#include <GL/gl.h>
#endif

#include "randomTargets.h"

#include <libgametrak/GameTrak.h>

using namespace std;
using namespace gametrak;

RandomTargets RT;

double HandsDistance = 0.0;

Vecteur3D Scale = Vecteur3D(0.02,0.02,0.02);
Vecteur3D Translate = Vecteur3D(0.0,-5.0,0.0);


// For targets
double nb = 10, xmin = -7, xmax = 7, ymin = 0, ymax = 15, zmin = -8, zmax = -2, sizemin = 0.5, sizemax = 1.0;


Vecteur3D gametrakLeftHand;
Vecteur3D gametrakRightHand;

bool debug = false;

GameTrak *gt = 0 ;
TimeStamp::inttime last_time = 0 ;
bool button_pressed = false ;

bool showhelp = true;

void
GameTrakCallback(void *context, 
             TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button)  {
    if (!gt) return ;

    gametrakLeftHand.x=leftx;
    gametrakLeftHand.y=lefty;
    gametrakLeftHand.z=leftz;

    gametrakRightHand.x=rightx;
    gametrakRightHand.y=righty;
    gametrakRightHand.z=rightz;

    button_pressed = button;

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
    glLineWidth(0.1);

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
    char txt[500];
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

    glPushMatrix();
    glTranslatef(0,-5,-20);
    glColor3f(1,1,1);
    Grid(20,30,0.5);

    RT.HitTarget(LeftP.x,LeftP.y,LeftP.z,button_pressed );
    RT.HitTarget(RightP.x,RightP.y,RightP.z,button_pressed );
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
        glColor3f(0,0,1);
        sprintf(txt,"%3.1f %3.1f %3.1f\r", LeftP.x, LeftP.y, LeftP.z);
        GLDisplayString(txt, 0.05,0.95,0.1);
        glColor3f(1,0,0);
        sprintf(txt,"%3.1f %3.1f %3.1f\r", RightP.x, RightP.y, RightP.z);
        GLDisplayString(txt, 0.2,0.95,0.1);
    }

    if (showhelp) {
        glColor3f(0,0,1);
        sprintf(txt,"[h] toggle help");
        GLDisplayString(txt, 0.8,0.95,0.1);
        sprintf(txt,"[f] toggle filtering");
        GLDisplayString(txt, 0.8,0.9,0.1);
        sprintf(txt,"[r] reinit targets");
        GLDisplayString(txt, 0.8,0.85,0.1);
        sprintf(txt,"[e] enter calibration");
        GLDisplayString(txt, 0.8,0.8,0.1);
        sprintf(txt,"[l] leave calibration");
        GLDisplayString(txt, 0.8,0.75,0.1);
        sprintf(txt,"[d] toggle debug");
        GLDisplayString(txt, 0.8,0.7,0.1);
        sprintf(txt,"[c] toggle calibration");
        GLDisplayString(txt, 0.8,0.65,0.1);
    }

    if (gt!=NULL) {
        if (gt->isCalibrating()) {
            glColor3f(1,0.5,0);
            //std::string s =  gt->getCalibrationString();
            sprintf(txt,"%s",gt->getCalibrationString().c_str());
            GLDisplayString(txt, 0.05,0.9,0.08);
        }
    }

#ifdef WIN32
	Sleep(20);
#else
    usleep(20000);
#endif
    glutSwapBuffers();
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
    case 'e':
        gt->enterCalibration();
        break;
    case 'l': {
        std::string calibString = gt->leaveCalibration();
        std::cout << calibString << std::endl; }
        break;
    case 'f':
        gt->toggleFiltering();
        break;
    case 'h':
        showhelp = !showhelp;
        break;
    case 'r':
        RT.ReStart();
        break;
    case 'c':
        gt->toggleCalibration();
        break;
    }
}

void idle(){
        glutPostRedisplay();
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
    {
        char txt[50];
        sprintf(txt, "%dx%d:24@60",glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        //glutGameModeString("1024x768:24@60");	//1024*768, 32bit pixel depth, 85Hz refresh rate
        glutGameModeString(txt);
        glutEnterGameMode();					//start fullscreen game mode
    }


    initGL();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    try {;
        gt = GameTrak::create(argc>1?argv[1]:"");
        gt->setGameTrakCallback(GameTrakCallback);
    } catch (std::runtime_error e) {
        std::cerr << "Runtime error: " << e.what() << std::endl ;
    } catch (std::exception e) {
        std::cerr << "Exception: " << e.what() << std::endl ;
    }

    glutMainLoop();

    return 0;
}
