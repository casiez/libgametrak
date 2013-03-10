/* -*- mode: c++ -*-
 *
 * libgametrak/GameTrak.h --
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

#ifndef GameTrak_h
#define GameTrak_h

#include <libgametrak/utils/Quaternion.h>
#include <libgametrak/utils/TimeStamp.h>
#include <libgametrak/utils/URI.h>
#include <libgametrak/utils/Quaternion.h>
#include <libgametrak/utils/OneEuroFilter.h>
#include <stdexcept>

namespace gametrak {

  class GameTrak {

  protected:
    // Raw values from encoders
    double rawLeftTheta, rawLeftPhi, rawLeftL;
    double rawRightTheta, rawRightPhi, rawRightL;

    bool button;

    // OneEuroFilter settings
    double mincutoff;
    double beta;
    double dcutoff;

    OneEuroFilter *filterLeftTheta, *filterLeftPhi, *filterLeftL;
    OneEuroFilter *filterRightTheta, *filterRightPhi, *filterRightL;

    // Raw values from encoders after filtering
    double rawLeftThetaf, rawLeftPhif, rawLeftLf;
    double rawRightThetaf, rawRightPhif, rawRightLf;

    // Metric values
    double LeftTheta; // degrees
    double LeftPhi; // degrees
    double LeftL; // mm

    double RightTheta; // degrees
    double RightPhi; // degrees
    double RightL; // mm

    // String ends positions
    double LeftX, LeftY, LeftZ;
    double RightX, RightY, RightZ;

    GameTrak(void) ;

    void FilterRawvalues(double timestamp);

    Vecteur3D Transform(double Theta, double Phi, double L) ;

  public:

    typedef void (*GameTrakCallback)(void *context, 
				     TimeStamp::inttime timestamp, 
                    double leftx, double lefty, double leftz,
				     double rightx, double righty, double rightz,
				     bool button) ;

    static GameTrak *create(const char *device_uri=0) ;
    static GameTrak *create(std::string device_uri) ;

    static void idle(int milliseconds) ;

    virtual bool isActive(void) const { return true ; }

    virtual URI getURI(bool expanded=false) const = 0 ;

    virtual void setGameTrakCallback(GameTrakCallback callback, void *context=0) = 0 ;

    virtual void setDebugLevel(int /*level*/) {}
    virtual void debug(std::ostream& /*out*/) const {}

    virtual ~GameTrak(void) {}

  } ;

}

#endif