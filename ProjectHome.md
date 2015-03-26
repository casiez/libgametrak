**April 11th, 2013: [First release version (v0.9)](http://code.google.com/p/libgametrak/downloads/list)**

**What is the Gametrak?**
[Wikipedia](http://en.wikipedia.org/wiki/Gametrak) [Review](http://cb.nowan.net/blog/2006/09/25/gametrak-a-first-impression/) [Music](http://www.youtube.com/watch?v=HFfR_9Wczjc) [Research project using the Gametrak](http://www.youtube.com/watch?v=ZxJD9DXDB1E)

<b>Features:</b>

<ul>
<li>Cross-platform library based on <a href='http://www.signal11.us/oss/hidapi/'>HIDAPI</a> to get raw events from the Gametrak (first you need to install a fresh version of HIDAPI from <a href='https://github.com/signal11/hidapi'>here</a>)</li>
<li>Inspired by <a href='http://www.libpointing.org/'>Libpointing</a></li>
<li>Get the position of each string end in Cartesian coordinates using metric units (mm)</li>
<li>Use callback functions instead of pooling</li>
<li><a href='http://www.lifl.fr/~casiez/1euro/'>1€</a> filter embedded</li>
<li>Hotplugging</li>
<li>Supports calibration procedure</li>
<li>Easy and powerful configuration through URI (read <a href='https://code.google.com/p/libgametrak/wiki/Documentation'>documentation</a>)</li>
<li>Easily connect several Gametraks to the same machine</li>
<li>Supports <a href='http://janoc.rd-h.com/archives/129'>Pictrak</a> mod by Jan Ciger</li>
</ul>

<b>Minimal example:</b>
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