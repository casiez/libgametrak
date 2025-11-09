/* -*- mode: c++ -*-
 *
 * libgametrak/HIDAPIGameTrak.h --
 *
 * Initial software
 * Authors: Géry Casiez (gery.casiez@univ-lille.fr)
 * Copyright University Lille, Inria
 *
 * https://github.com/casiez/libgametrak
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
  #include <unistd.h>
#endif

namespace gametrak {

  class HIDAPIGameTrak : public GameTrak {
  private:
  	hid_device *handle;
    GameTrakCallback callback ;
    void *callback_context ;

    bool run; // for the Loop thread

    bool pictrak; // Jan Ciger Pictrak board
    bool ps2mode;
    unsigned int ps2key;
    unsigned char ps2index;
    std::string serial_number;

    bool threadFinished;

#ifdef WIN32
    HANDLE hThreads[1];
    DWORD dwThreadId;
    DWORD dwThreadParam;
#else
    pthread_t thread ;
#endif

    std::string devicePath;

    double rawLeftThetafPrev, rawLeftPhifPrev, rawLeftLfPrev,
      rawRightThetafPrev, rawRightPhifPrev, rawRightLfPrev;

    void connect();
    void disconnect();
    void ps2Init(bool reset);
    void ps2NextKey();

    volatile bool deviceConnected;

  public:
  
    HIDAPIGameTrak(URI uri) ;

    URI getURI(bool expanded=false) const ;
#ifdef WIN32
    static DWORD WINAPI eventloop(LPVOID lpvThreadParam);
#else
    static void *eventloop(void *self) ;
#endif

    void setGameTrakCallback(GameTrakCallback callback, void *context) ;

    ~HIDAPIGameTrak() ;

  } ;

}

#endif
