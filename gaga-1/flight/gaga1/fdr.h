// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These functions are used to store values in the EEPROM in the Arduino.  Since
// this memory is non-volatile we can read it even after a power loss.

#ifndef INCLUDED_FDR
#define INCLUDED_FDR

#include "WProgram.h"

unsigned int fdr_init();
void fdr_max_altitude( unsigned int alt );
void fdr_min_internal( int temp );
void fdr_min_external( int temp );

#endif // INCLUDED_FDR
