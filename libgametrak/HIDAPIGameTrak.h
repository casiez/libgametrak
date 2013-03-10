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
#include <pthread.h>


namespace gametrak {

  class HIDAPIGameTrak : public GameTrak {
  private:
  	hid_device *handle;
  	pthread_t thread ;
    GameTrakCallback callback ;
    void *callback_context ;

  public:
  
    HIDAPIGameTrak(URI uri) ;

    URI getURI(bool expanded=false) const ;

    static void *eventloop(void *self) ;

    void setGameTrakCallback(GameTrakCallback callback, void *context) ;

    ~HIDAPIGameTrak() {}

  } ;

}

#endif