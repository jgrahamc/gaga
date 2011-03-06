// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Basic functions for handling time.  The flight computer needs to know
// elapsed time in seconds since the flight started.

#include "time.h"

// Used to store the start time (set when time_init() is called). This
// is the zero time for time_elapsed()

unsigned long start_time;

// time_seconds: get the current time in seconds
unsigned long time_seconds()
{
  return millis() / 1000;
}

// time_init: mark now as time 0
void time_init()
{
  start_time = time_seconds();
}

// time_elapsed: get the number of seconds since time_init() was called
unsigned long time_elapsed()
{
  return time_seconds() - start_time;
}

// time_format: returns a string containing the time in seconds
char * time_format( unsigned long elapsed ) // Time returned by time_elapsed()
{
#define MAX_TIME_STRING 32
  static char time_string[MAX_TIME_STRING+1];

  sprintf( time_string, "%ld,", elapsed );
  
  return time_string;
}

