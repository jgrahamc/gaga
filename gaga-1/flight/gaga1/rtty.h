// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These are RTTY functions that send RTTY using the attached
// Radiometrix NTX2

#ifndef INCLUDED_RTTY
#define INCLUDED_RTTY

#include "WProgram.h"

void rtty_init();
void rtty_send( char * );

#endif // INCLUDED_RTTY
