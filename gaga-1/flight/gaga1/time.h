// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Basic functions for handling time.  The flight computer needs to know
// elapsed time in seconds since the flight started.

#ifndef INCLUDED_TIME
#define INCLUDED_TIME

#include "WProgram.h"

void time_init();
char * time_format( unsigned long elapsed );
unsigned long time_elapsed();

#endif // INCLUDED_TIME

