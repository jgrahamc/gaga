// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Functions to access an Lassen IQ module via its TSIP interface
// on the Arduino's serial connection

#ifndef INCLUDED_TSIP
#define INCLUDED_TSIP

#include "WProgram.h"

void tsip_init();
void tsip_handle();

struct position {
  float latitude;   // Latitude in radians (- = south)
  float longitude;  // Longistude in radians (- = west)
  float altitude;   // Altitude in meters
  float vertical;   // Vertical speed in meters/second (- = down)
  float fix;        // UTC time of last fix
};

struct position tsip_get();

#endif // INCLUDED_TSIP
