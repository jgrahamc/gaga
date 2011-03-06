// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These functions to interface to the Lassen IQ GPS module using
// TSIP.  Very basic functions to get current location/time and
// intialize the GPS are provided.

#include "gps.h"
#include "tsip.h"
#include "fdr.h"

// Last position received from the GPS with latitude and longitude
// converted to degrees
struct position last_fix; 

// gps_init: 
void gps_init()
{
  last_fix.latitude  = 0;
  last_fix.longitude = 0;
  last_fix.altitude  = 0;
  last_fix.vertical  = 0;
  last_fix.fix       = 0;
  
  tsip_init();
}

#define MAX_GETTER_STRING 32
#define GETTER(_a,_b)                         \
char * gps_##_a()                             \
{                                             \
  static char string[MAX_GETTER_STRING+1];    \
  sprintf( string, _b, last_fix._a );         \
  return string;                              \
}

// gps_latitude: return the current latitude in format +DD.dddd,
GETTER(latitude,"%2.4d,")

// gps_longitude: return the current longitude in format +DDD.dddd,
GETTER(longitude,"%3.4d,")

// gps_altitude: return the current altitude
GETTER(altitude,"%d,")

// gps_vertical: return the vertical speed in format +DD.dd
GETTER(vertical,"%2.2d,")

// gps_fix: return the time of last fix in format HHmmss
char * gps_fix()
{
  char string[MAX_GETTER_STRING+1];
  
  int fix = last_fix.fix;
  sprintf( string, "%2d%2d%2d,", fix / (60 * 60),
    (fix / 60) % 60, fix % 60 );
   
   return string; 
}


// gps_update: ensure that the latest fix has been obtained from the GPS
void gps_update()
{
  tsip_handle();
   
  last_fix = tsip_get();
 
#define RADTODEG(_a) last_fix._a = 360 * last_fix._a / 6.2818
 
 RADTODEG(latitude);
 RADTODEG(longitude);
 
 // The .fix value is the UTC time since the start of the GPS week in seconds
 // We need to convert this to HHmmss in the current day.
 
 int fix = last_fix.fix;
 fix %= 24 * 60 * 60;
 last_fix.fix = fix; 
 
 fdr_max_altitude( int(last_fix.fix) );
}
