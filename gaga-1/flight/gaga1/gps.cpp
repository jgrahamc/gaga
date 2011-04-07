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
  gps_clear();  
  tsip_init();
}

// gps_clear: clear the last fix information 
void gps_clear()
{
  last_fix.latitude  = 0;
  last_fix.longitude = 0;
  last_fix.altitude  = 0;
  last_fix.vertical  = 0;
  last_fix.fix       = 0;
}

#define MAX_GETTER_STRING 32

// gps_latitude: return the current latitude in format +DD.dddd,
char * gps_latitude()
{
  static char string[MAX_GETTER_STRING+1];
  dtostrf( last_fix.latitude, 7, 4, string );
  strcat( string, "," );
  if ( string[0] == ' ' ) {
    return &string[1];
  } else {
    return string;
  }  
  return string;
}

// gps_longitude: return the current longitude in format +DDD.dddd,
char * gps_longitude()
{
  static char string[MAX_GETTER_STRING+1];
  dtostrf( last_fix.longitude, 7, 4, string );
  strcat( string, "," );
  if ( string[0] == ' ' ) {
    return &string[1];
  } else {
    return string;
  }  
}

// gps_altitude: return the current altitude
char * gps_altitude()
{
  static char string[MAX_GETTER_STRING+1];
  int l = int(last_fix.altitude);
  sprintf( string, "%d,", l );
  return string;
}

// gps_vertical: return the vertical speed in format +DD.dd
char * gps_vertical()
{
  static char string[MAX_GETTER_STRING+1];
  dtostrf( last_fix.vertical, 5, 2, string );
  strcat( string, "," );
  if ( string[0] == ' ' ) {
    return &string[1];
  } else {
    return string;
  }  
}

// gps_fix: return the time of last fix in format HHmmss
char * gps_fix()
{
  char string[MAX_GETTER_STRING+1];

  long fix = long(last_fix.fix);  
  long day = 86400;
   
  while ( fix > day ) {
    fix -= day;
  }   
  
  long hours = fix / 3600;
  fix -= hours * 3600;
  long mins  = fix / 60;
  long secs  = fix - ( mins * 60 ); 
  sprintf( string, "%02ld:%02ld:%02ld,", hours, mins, secs );
  return string; 
}


// gps_update: ensure that the latest fix has been obtained from the GPS
void gps_update()
{
  tsip_handle();
   
  last_fix = tsip_get();
 
#define RADTODEG(_a) last_fix._a = 360.0 * last_fix._a / ( 2 * 3.1415926535898 )
 
 RADTODEG(latitude);
 RADTODEG(longitude);
 
 // Ensure that we only keep the integer part of altitude
 
 last_fix.altitude = floor(last_fix.altitude);
 
 fdr_max_altitude( int(last_fix.altitude) );
}

// gps_have_fix: returns 1 if we have a fix, 0 if not
int gps_have_fix()
{
  return ( last_fix.fix != 0 );
}

