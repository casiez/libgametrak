/* -*- mode: c++ -*-
 *
 * Demos/ConsoleExample/consoleExample.cpp --
 *
 * Initial software
 * Authors: Gery Casiez (gery.casiez@univ-lille.fr)
 * Copyright University Lille, Inria
 *
 * https://github.com/casiez/libgametrak
 *
 * This software may be used and distributed according to the terms of
 * the GNU General Public License version 2 or any later version.
 *
 */

#include <libgametrak/GameTrak.h>
#include <iomanip>
#include <stdexcept>

using namespace gametrak ;

//////////////////////////////////////////////////////////
/*
 * If usePullMode == true then the callBack is not called,
 * the cursor(s) positions must be updated explicitly by
 * calling getters (as in the example at the begining of
 * the function "display()"
 */
bool usePullMode = false;//true;
//////////////////////////////////////////////////////////

GameTrak *gt = 0 ;
TimeStamp::inttime last_time = 0 ;
bool button_pressed = false ;

void printInfo(TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button) {
  double freq = 1/((timestamp - last_time) * 1.0E-9);
  std::cout << timestamp << " ns, " 
	    << std::setw(7) << freq << " Hz, "
	    << "Left: (" << std::setw(3) << leftx << ", " << std::setw(3) << lefty << ", " << std::setw(3) << leftz << ") mm"
	    << ", Right: (" << std::setw(3) << rightx << ", " << std::setw(3) << rightx << ", " << std::setw(3) << rightz << ") mm, "
	    << "button: " << button << std::endl ;
}

void
GameTrakCallback(void *context, 
             TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button)  {
  if (!gt) return ;

  /*double freq = 1/((timestamp - last_time) * 1.0E-9);
  std::cout << timestamp << " ns, " 
	    << std::setw(7) << freq << " Hz, "
	    << "Left: (" << std::setw(3) << leftx << ", " << std::setw(3) << lefty << ", " << std::setw(3) << leftz << ") mm"
	    << ", Right: (" << std::setw(3) << rightx << ", " << std::setw(3) << rightx << ", " << std::setw(3) << rightz << ") mm, "
	    << "button: " << button << std::endl ;*/
  printInfo(timestamp,leftx,lefty,leftz,rightx,righty,rightz,button);
  last_time = timestamp;
  button_pressed = button;
}

int
main(int argc, char** argv) {
  try {

    if (!usePullMode) {
       gt = GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1&ps2mode=true") ;
       gt->setGameTrakCallback(GameTrakCallback) ;
    } else {
       gt = GameTrak::create(argc>1?argv[1]:"any:?pullMode=true") ;
    }
    if (!gt->isGametrakConnected()) {
       std::cerr << "No Gametrak Connected !" << std::endl ;
       return 1;
    }
    //gt= GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1&pictrak=true") ;
    //gt= GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1&devicePath=USB_05ac_0237_fa120000") ;

    if (!usePullMode) {
        while (!button_pressed)
          GameTrak::idle(100) ; // milliseconds
    } else {
        TimeStamp::inttime timestamp;
        double lX, lY, lZ;
        double rX, rY, rZ;

        while (!button_pressed) {
          if (gt) {
              gt->getGametrakData(&timestamp,&lX,&lY,&lZ,&rX,&rY,&rZ,&button_pressed);
              if (timestamp != last_time) {
                  printInfo(timestamp,lX,lY,lZ,rX,rY,rZ,button_pressed);
                  last_time = timestamp;
              }
          }
        }
    }

    delete gt ;

  } catch (std::runtime_error e) {
    std::cerr << "Runtime error: " << e.what() << std::endl ;
  } catch (std::exception e) {
    std::cerr << "Exception: " << e.what() << std::endl ;
  }

  return 0 ;
}
