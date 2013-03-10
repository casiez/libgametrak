/* -*- mode: c++ -*-
 *
 * libgametrak/HIDAPIGameTrak.cpp --
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

#include <libgametrak/HIDAPIGameTrak.h>

#include <sstream>

namespace gametrak {


  HIDAPIGameTrak::HIDAPIGameTrak(URI uri):GameTrak() {

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    handle = hid_open(0x14B7, 0x0982, NULL);
    if (handle == NULL) {
      throw std::runtime_error("HIDAPIGameTrak: pb opening GameTrak") ;
    }

    callback = 0 ;
    callback_context = 0 ;

    int ret = pthread_create(&thread, NULL, eventloop, (void*)this) ;
    if (ret<0) {
      perror("HIDAPIGameTrak::HIDAPIGameTrak") ;
      throw std::runtime_error("HIDAPIGameTrak: pthread_create failed") ;    
    }
  }


  void *
  HIDAPIGameTrak::eventloop(void *context) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) ;
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) ;

    HIDAPIGameTrak *self = (HIDAPIGameTrak*)context ;

    hid_device *handle = self->handle;

    unsigned char buf[16];

    while (true) {
      // Read requested state
      int res = hid_read(handle, buf, 16);
      if (res < 0)
        throw std::runtime_error("HIDAPIGameTrak: hid_read error") ; 

      self->rawLeftTheta = (buf[15] << 8) + buf[14];
      self->rawLeftPhi = (buf[3] << 8) + buf[2];
      self->rawLeftL = (buf[5] << 8) + buf[4];

      self->rawRightTheta = (buf[7] << 8) + buf[8];
      self->rawRightPhi = (buf[9] << 8) + buf[6];
      self->rawRightL = (buf[11] << 8) + buf[10];

      bool button = buf[12] == 16;

      TimeStamp::inttime now = TimeStamp::createAsInt();

      self->FilterRawvalues(now * 1.0E-9);

      if (self->callback != 0)
        self->callback(self->callback_context, now, self->rawLeftTheta, self->rawLeftPhi, self->rawLeftL, self->rawRightTheta, self->rawRightPhi, self->rawRightL, button);


      // Metric values

      double mid = 4096.0/2.0;
      double angleMax = 30.0; // * M_PI / 180.0;

      self->LeftTheta = -(self->rawLeftThetaf - mid) * angleMax / mid;
      self->LeftPhi = -(self->rawLeftPhif - mid) * angleMax / mid;
      self->LeftL = - 0.041 * self->rawLeftLf + 2839;

      self->RightTheta = (self->rawRightThetaf - mid) * angleMax / mid;
      self->RightPhi = -(self->rawRightPhif - mid) * angleMax / mid;
      self->RightL = - 0.041 * self->rawRightLf + 2839; 

      Vecteur3D LeftHand = self->Transform(self->LeftTheta * M_PI / 180.0, self->LeftPhi * M_PI / 180.0, self->LeftL);
      self->LeftX = LeftHand.x;
      self->LeftY = LeftHand.y;
      self->LeftZ = LeftHand.z;

      Vecteur3D RightHand = self->Transform(self->RightTheta * M_PI / 180.0, self->RightPhi * M_PI / 180.0, self->RightL);
      self->RightX = RightHand.x;
      self->RightY = RightHand.y;
      self->RightZ = RightHand.z;

      // if (self->callback != 0)
      //   self->callback(self->callback_context, now, self->LeftX, self->LeftY, self->LeftZ, self->RightX, self->RightY, self->RightZ, button);
    }

    return 0 ;
  }

  void
  HIDAPIGameTrak::setGameTrakCallback(GameTrakCallback cbck, void *ctx) {
    callback = cbck ;
    callback_context = ctx ;
  }


  URI
  HIDAPIGameTrak::getURI(bool expanded) const {
    URI uri ;
    uri.scheme = "dummy" ;
    // int i = 0 ;
    // std::stringstream q ;
    // if (expanded || hz!=DUMMY_DEFAULT_HZ) 
    //   q << (i++?"&":"") << "hz=" << hz ;
    // if (expanded || cpi!=DUMMY_DEFAULT_CPI) 
    //   q << (i++?"&":"") << "cpi=" << cpi ;
    // uri.query = q.str() ;
    return uri ;
  }

}
