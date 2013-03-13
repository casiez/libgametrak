/* -*- mode: c++ -*-
 *
 * libgametrak/GameTrak.cpp --
 *
 * Initial software
 * Authors: Géry Casiez (gery.casiez@lifl.fr)
 * Copyright University Lille 1, Inria
 *
 * https://code.google.com/p/libgametrak/
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#include <libgametrak/GameTrak.h>
#include <libgametrak/HIDAPIGameTrak.h>

#ifdef __APPLE__
  #include <CoreServices/CoreServices.h>
#endif

 namespace gametrak {

  GameTrak::GameTrak(void) {
    rawLeftTheta = 0;
    rawLeftPhi = 0;
    rawLeftL = 0;
    rawRightTheta = 0;
    rawRightPhi = 0;
    rawRightL = 0;
    button = false;

    mincutoff = 1.0; 
    beta = 0.001;
    dcutoff = 1.0;

    filterLeftTheta = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;
    filterLeftPhi = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;
    filterLeftL = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;
    filterRightTheta = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;
    filterRightPhi = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;
    filterRightL = new OneEuroFilter(120, mincutoff, beta, dcutoff) ;

    rawLeftThetaf = 0;
    rawLeftPhif = 0;
    rawLeftLf = 0;
    rawRightThetaf = 0;
    rawRightPhif = 0;
    rawRightLf = 0;

    LeftTheta = 0; // degrees
    LeftPhi = 0; // degrees
    LeftL = 0; // mm

    RightTheta = 0; // degrees
    RightPhi = 0; // degrees
    RightL = 0; // mm

    // String ends positions
    LeftX = 0;
    LeftY = 0;
    LeftZ = 0;
    RightX = 0;
    RightY = 0;
    RightZ = 0;

  }

  GameTrak *
  GameTrak::create(const char *device_uri) {
    std::string uri ;
    if (device_uri) uri = device_uri ;
    return create(uri) ;
  }

  GameTrak *
  GameTrak::create(std::string device_uri) {
  	return new HIDAPIGameTrak(device_uri);
  }

  void GameTrak::FilterRawvalues(double timestamp) {
    rawLeftThetaf = filterLeftTheta->filter(rawLeftTheta, timestamp);
    rawLeftPhif = filterLeftPhi->filter(rawLeftPhi, timestamp);
    rawLeftLf = filterLeftL->filter(rawLeftL, timestamp);
    rawRightThetaf = filterRightTheta->filter(rawRightTheta, timestamp);
    rawRightPhif = filterRightPhi->filter(rawRightPhi, timestamp);
    rawRightLf = filterRightL->filter(rawRightL, timestamp);

  }

  Vecteur3D GameTrak::Transform(double Theta, double Phi, double L)
  {
    Quaternion Qtheta, Qphi, Qres;
    Qtheta.AxisToQuaternion(Vecteur3D(0,0,1),Theta);
    Qphi.AxisToQuaternion(Vecteur3D(1,0,0),Phi);
    Qres = Qtheta * Qphi;
    return Qres.RotateVector(Vecteur3D(0,L,0));
  }

  void
  GameTrak::idle(int milliseconds) {
#ifdef WIN32
    Sleep(milliseconds) ;
#endif
#ifdef __linux__
    usleep(milliseconds*1000) ;
#endif
#ifdef __APPLE__
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, milliseconds/1000.0, false) ;
#endif 
  }

}
