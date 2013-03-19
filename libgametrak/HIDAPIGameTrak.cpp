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


    // Enumerate and print the HID devices on the system
    struct hid_device_info *devs, *cur_dev;
    
    //hid_init();
    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs; 
    while (cur_dev) {
      std::wstring product_string(cur_dev->product_string);
      if (product_string == L"Game-Trak V1.3") {
        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
          cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        printf("\n");
        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("\n");
      }
      cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    devicePath = "";
    URI::getQueryArg(uri.query, "devicePath", &devicePath) ;

    if (devicePath != "") {
      // Open the device using device path
      handle = hid_open_path(devicePath.c_str());
    } else {
      // Open the device using the VID, PID,
      // and optionally the Serial number.
      try {
        handle = hid_open(0x14B7, 0x0982, NULL);
        if (handle == NULL) {
          throw std::runtime_error("HIDAPIGameTrak: pb opening GameTrak") ;
        }
      } catch (std::exception e) {
        std::cerr << "Exception with hid_open: " << e.what() << std::endl ;
      }
    }


    rawLeftThetafPrev = 0;
    rawLeftPhifPrev = 0;
    rawLeftLfPrev = 0;
    rawRightThetafPrev = 0;
    rawRightPhifPrev = 0;
    rawRightLfPrev = 0;

    callback = 0 ;
    callback_context = 0 ;

    URI::getQueryArg(uri.query, "milt", &minRawLeftTheta) ;
    URI::getQueryArg(uri.query, "milp", &minRawLeftPhi) ;
    URI::getQueryArg(uri.query, "mill", &minRawLeftL) ;
    URI::getQueryArg(uri.query, "mirt", &minRawRightTheta) ;
    URI::getQueryArg(uri.query, "mirp", &minRawRightPhi) ;
    URI::getQueryArg(uri.query, "mirl", &minRawRightL) ;

    URI::getQueryArg(uri.query, "malt", &maxRawLeftTheta) ;
    URI::getQueryArg(uri.query, "malp", &maxRawLeftPhi) ;
    URI::getQueryArg(uri.query, "mall", &maxRawLeftL) ;
    URI::getQueryArg(uri.query, "mart", &maxRawRightTheta) ;
    URI::getQueryArg(uri.query, "marp", &maxRawRightPhi) ;
    URI::getQueryArg(uri.query, "marl", &maxRawRightL) ;

    run = true;

#ifdef WIN32
  hThreads[0]=CreateThread(NULL, NULL, eventloop, LPVOID(this), 0, &dwThreadId);
#else
    int ret = pthread_create(&thread, NULL, eventloop, (void*)this) ;
    if (ret<0) {
      perror("HIDAPIGameTrak::HIDAPIGameTrak") ;
      throw std::runtime_error("HIDAPIGameTrak: pthread_create failed") ;    
    }
#endif
  }

#ifdef WIN32
DWORD WINAPI HIDAPIGameTrak::eventloop(LPVOID context)
{
#else
  void *
  HIDAPIGameTrak::eventloop(void *context) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) ;
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) ;
#endif

    HIDAPIGameTrak *self = (HIDAPIGameTrak*)context ;

    hid_device *handle = self->handle;

    unsigned char buf[16];

    while (self->run) {
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

      // If position changed then call the callback
      /*
      bool send = false;
      if ((floor(self->rawLeftThetaf) != self->rawLeftThetafPrev) ||
          (floor(self->rawLeftPhif) != self->rawLeftPhifPrev) ||
          (floor(self->rawLeftLf) != self->rawLeftLfPrev) ||
          (floor(self->rawRightThetaf) != self->rawRightThetafPrev) ||
          (floor(self->rawRightPhif) != self->rawRightPhifPrev) ||
          (floor(self->rawRightLf) != self->rawRightLfPrev)) {
          send = true;
       }

      self->rawLeftThetafPrev = floor(self->rawLeftThetaf);
      self->rawLeftPhifPrev = floor(self->rawLeftPhif);
      self->rawLeftLfPrev = floor(self->rawLeftLf);
      self->rawRightThetafPrev = floor(self->rawRightThetaf);
      self->rawRightPhifPrev = floor(self->rawRightPhif);
      self->rawRightLfPrev = floor(self->rawRightLf);
    */

      if (self->calibrating) {
        self->calibrate();
      }


      // Metric values
      double angleMax = 30.0; // degrees - need to be better measured
      double stringLength = 3000.0; // mm - need to be better measured
      double distance2strings = 100.0; // mm - need to be better measured

      if (self->calibrated) {
        double mid = 4096.0/2.0;

        double midLeftTheta = self->maxRawLeftTheta - self->minRawLeftTheta;
        self->LeftTheta = (self->rawLeftThetaf - midLeftTheta) * angleMax / midLeftTheta;
        double midLeftPhi = self->maxRawLeftPhi - self->minRawLeftPhi;
        self->LeftPhi = -(self->rawLeftPhif - midLeftPhi) * angleMax / midLeftPhi;
        self->LeftL = - stringLength/(self->maxRawLeftL - self->minRawLeftL) * self->rawLeftLf + stringLength;

        double midRightTheta = self->maxRawRightTheta - self->minRawRightTheta;
        self->RightTheta = -(self->rawRightThetaf - midRightTheta) * angleMax / midRightTheta;
        double midRightPhi = self->maxRawRightPhi - self->minRawRightPhi;
        self->RightPhi = -(self->rawRightPhif - midRightPhi) * angleMax / midRightPhi;
        self->RightL = - stringLength/(self->maxRawRightL - self->minRawRightL) * self->rawRightLf + stringLength; 
      } else { 
        double mid = 4096.0/2.0;

        self->LeftTheta = (self->rawLeftThetaf - mid) * angleMax / mid;
        self->LeftPhi = -(self->rawLeftPhif - mid) * angleMax / mid;
        self->LeftL = - stringLength/4096.0 * self->rawLeftLf + stringLength;

        self->RightTheta = -(self->rawRightThetaf - mid) * angleMax / mid;
        self->RightPhi = -(self->rawRightPhif - mid) * angleMax / mid;
        self->RightL = - stringLength/4096.0 * self->rawRightLf + stringLength; 
      }

      Vecteur3D LeftHand = self->Transform(self->LeftTheta * M_PI / 180.0, self->LeftPhi * M_PI / 180.0, self->LeftL);
      self->LeftX = LeftHand.x - distance2strings / 2.0;
      self->LeftY = LeftHand.y;
      self->LeftZ = LeftHand.z;

      Vecteur3D RightHand = self->Transform(self->RightTheta * M_PI / 180.0, self->RightPhi * M_PI / 180.0, self->RightL);
      self->RightX = RightHand.x + distance2strings / 2.0;
      self->RightY = RightHand.y;
      self->RightZ = RightHand.z;

      // bool send = true;
      // if (send && (self->callback != 0))
      //   self->callback(self->callback_context, now, floor(self->rawLeftThetaf), floor(self->rawLeftPhif), floor(self->rawLeftLf), floor(self->rawRightThetaf), floor(self->rawRightPhif), floor(self->rawRightLf), button);

      // bool send = true;
      // if (send && (self->callback != 0))
      //   self->callback(self->callback_context, now, self->LeftTheta, self->LeftPhi, self->LeftL, self->RightTheta, self->RightPhi, self->RightL, button);

      bool send = true;
      if (send && (self->callback != 0))
        self->callback(self->callback_context, now, self->LeftX, self->LeftY, self->LeftZ, self->RightX, self->RightY, self->RightZ, button);
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
    uri.scheme = "hidapigt" ;
    // int i = 0 ;
    // std::stringstream q ;
    // if (expanded || hz!=DUMMY_DEFAULT_HZ) 
    //   q << (i++?"&":"") << "hz=" << hz ;
    // if (expanded || cpi!=DUMMY_DEFAULT_CPI) 
    //   q << (i++?"&":"") << "cpi=" << cpi ;
    // uri.query = q.str() ;
    return uri ;
  }

  HIDAPIGameTrak::~HIDAPIGameTrak() {
    run = false;
  }

}
