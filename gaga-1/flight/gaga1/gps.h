// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These functions to interface to the Lassen IQ GPS module using
// TSIP.  Very basic functions to get current location/time and
// intialize the GPS are provided.

#ifndef INCLUDED_GPS
#define INCLUDED_GPS

#include "WProgram.h"

void gps_init();
void gps_update();

char * gps_latitude();
char * gps_altitude();
char * gps_longitude();
char * gps_vertical();
char * gps_fix();

#endif // INCLUDED_GPS
