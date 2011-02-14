// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Code to interface to the DS1821 temperature sensor

#ifndef INCLUDED_DS1821
#define INCLUDED_DS1821

#include "WProgram.h"

int ds1821_read_bits( int bits, int pin );
int ds1821_write_bits( int bits, int w, int pin );

#endif // INCLUDED_DS1821


