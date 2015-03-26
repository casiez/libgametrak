What is the Gametrak? Wikipedia Review Music Research project using the Gametrak

Features:

    Cross-platform library based on HIDAPI to get raw events from the Gametrak (first you need to install a fresh version of HIDAPI from here)
    Inspired by Libpointing
    Get the position of each string end in Cartesian coordinates using metric units (mm)
    Use callback functions instead of pooling
    1€ filter embedded
    Hotplugging
    Supports calibration procedure
    Easy and powerful configuration through URI (read documentation)
    Easily connect several Gametraks to the same machine
    Supports Pictrak mod by Jan Ciger

Minimal example:

#include <libgametrak/GameTrak.h>
#include <iomanip>
#include <stdexcept>

using namespace gametrak ;

GameTrak *gt = 0 ;
TimeStamp::inttime last_time = 0 ;
bool button_pressed = false ;

void GameTrakCallback(void *context, 
             TimeStamp::inttime timestamp, 
             double leftx, double lefty, double leftz,
             double rightx, double righty, double rightz,
             bool button)  {
  if (!gt) return;
  double freq = 1/((timestamp - last_time) * 1.0E-9);
  std::cout << timestamp << " ns, " 
            << std::setw(7) << freq << " Hz, "
            << "Left: (" << std::setw(3) << leftx << ", " << std::setw(3) << lefty 
            << ", " << std::setw(3) << leftz << ") mm"
            << ", Right: (" << std::setw(3) << rightx << ", " << std::setw(3) << rightx
            << ", " << std::setw(3) << rightz << ") mm, "
            << "button: " << button << std::endl ;
  last_time = timestamp;
  button_pressed = button ;
}

int main(int argc, char** argv) {
  try {

    gt = GameTrak::create(argc>1?argv[1]:"") ;
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
