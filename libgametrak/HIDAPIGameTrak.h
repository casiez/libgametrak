/* -*- mode: c++ -*-
 *
 * libgametrak/HIDAPIGameTrak.h --
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

#ifndef HIDAPIGameTrak_h
#define HIDAPIGameTrak_h

#include <libgametrak/GameTrak.h>
#include <hidapi.h>
#include <stdio.h>

#ifdef WIN32
  #include <windows.h> 
#else
  #include <pthread.h>
#endif

namespace gametrak {

  class HIDAPIGameTrak : public GameTrak {
  private:
  	hid_device *handle;
    GameTrakCallback callback ;
    void *callback_context ;

#ifdef WIN32
	HANDLE hThreads[1];
	DWORD dwThreadId;
	DWORD dwThreadParam;
#else
  	pthread_t thread ;
#endif

    double rawLeftThetafPrev, rawLeftPhifPrev, rawLeftLfPrev,
      rawRightThetafPrev, rawRightPhifPrev, rawRightLfPrev;

  public:
  
    HIDAPIGameTrak(URI uri) ;

    URI getURI(bool expanded=false) const ;
#ifdef WIN32
	static DWORD WINAPI eventloop(LPVOID lpvThreadParam);
#else
    static void *eventloop(void *self) ;
#endif

    void setGameTrakCallback(GameTrakCallback callback, void *context) ;

    ~HIDAPIGameTrak() {}

  } ;

}

#endif
