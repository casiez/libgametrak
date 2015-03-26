#What is the Gametrak?
[Wikipedia](http://en.wikipedia.org/wiki/Gametrak) [Review](http://cb.nowan.net/blog/2006/09/25/gametrak-a-first-impression/) [Music](http://www.youtube.com/watch?v=HFfR_9Wczjc) [Research project using the Gametrak](http://www.youtube.com/watch?v=ZxJD9DXDB1E)

##Features
* Cross-platform library based on [HIDAPI](http://www.signal11.us/oss/hidapi/) to get raw events from the Gametrak (first you need to install a fresh version of HIDAPI from here)
* Inspired by [Libpointing](http://www.libpointing.org/)
* Get the position of each string end in Cartesian coordinates using metric units (mm)
* Use callback functions instead of pooling
* [1€](http://cristal.univ-lille.fr/~casiez/1euro/) filter embedded
* Hotplugging
* Supports calibration procedure
* Easy and powerful configuration through URI
* Easily connect several Gametraks to the same machine
* Supports [Pictrak](http://janoc.rd-h.com/archives/129) mod by Jan Ciger

##Minimal example
```
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
```
##URI documentation

uri are in the form "scheme:?arg1=value1&arg2=value2..."

Parameters can be set in arbitrary order. Parameters not set use default values.

###Default parameters
The uri "" sets all parameters to default

###Debug level
  * *debugLevel* (int) sets the debug level. Default is 0.
    * 1 and above: lists connected Gametraks and their related information (e.g. path, serial number) 
    * 2 and above: display metric values for each degree of freedom
    * 3 and above: display raw sensor information 

###Device parameters
  * *devicePath* (string) sets the device by its path. Default value: "". Use debugLevel 1 to know the path
  * *serial_number* (string) sets the device using its serial number. Default value: "". Only works with Pictrak.

###Filtering
  * *filter* (bool) sets filtering on or off. Default value: true
  * *mincutoff* (double) sets € filter min cutoff frequency. Default value: 1.0
  * *beta* (double) sets 1€ filter beta value. Default value: 0.001
  * *dcutoff* (double) sets 1€ filter dcutoff. Default value: 1.0 (should not be changed)

###Calibration
  * *useCalibration* (bool) defines if calibration is used. Default value: false
  * *milt*, *milp*, *mill*, *mirt*, *mirp*, *mirl*, *malt*, *malp*, *mall*, *mart*, *marp*, *marl* (double). Min (*mi*) and max (*ma*) values for left (*l*) and right (*r*) theta (*t*), phi (*p*) and L (*l*).

###Mode (for data processing)
  * *usePullMode* (bool) defines if the datas are available using getters (true) or using a callback function (false). Default value: false

###Connection
  * *nbOfTryMax* (int) defines a maximum number of try for connecting a Gametrak. Default value: -1 (infinite)

##Installation
Works on Windows, OS X, Linux

There is no driver to install, simply install the last version of [HIDAPI](https://github.com/signal11/hidapi)

##Frame of reference
The picture below illustrates the origin and the axis used to locate each Gametrak pointer. 
![Gametrak frame of reference](https://raw.githubusercontent.com/casiez/libgametrak/master/Doc/gametrak_axes.jpg)
