/* -*- mode: c++ -*-
 *
 * Demos/ConsoleExample/consoleExample.cpp --
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

#include <libgametrak/GameTrak.h>
#include <iomanip>
#include <stdexcept>

using namespace gametrak ;

GameTrak *gt = 0 ;
TimeStamp::inttime last_time = 0 ;
bool button_pressed = false ;

void
GameTrakCallback(void *context, 
             TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button)  {
  if (!gt) return ;

  double freq = 1/((timestamp - last_time) * 1.0E-9);
  std::cout << timestamp << " ns, " 
	    << std::setw(7) << freq << " Hz, "
	    << "Left: (" << std::setw(3) << leftx << ", " << std::setw(3) << lefty << ", " << std::setw(3) << leftz << ") mm"
	    << ", Right: (" << std::setw(3) << rightx << ", " << std::setw(3) << rightx << ", " << std::setw(3) << rightz << ") mm, "
	    << "button: " << button << std::endl ;
  last_time = timestamp;
  button_pressed = button ;
}

int
main(int argc, char** argv) {
  try {

    gt= GameTrak::create(argc>1?argv[1]:"") ;
    //gt= GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1&pictrak=true") ;
    //gt= GameTrak::create(argc>1?argv[1]:"any:?debugLevel=1&devicePath=USB_05ac_0237_fa120000") ;
    gt->setGameTrakCallback(GameTrakCallback) ;

    while (!button_pressed)
      GameTrak::idle(100) ; // milliseconds

    delete gt ;

  } catch (std::runtime_error e) {
    std::cerr << "Runtime error: " << e.what() << std::endl ;
  } catch (std::exception e) {
    std::cerr << "Exception: " << e.what() << std::endl ;
  }

  return 0 ;
}
