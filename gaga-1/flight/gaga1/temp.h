// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These are functions for reading temperatures from the internal and
// external DS1821+ sensors using the OneWire library

#ifndef INCLUDED_TEMP
#define INCLUDED_TEMP

#include "WProgram.h"

void temp_init();
int temp_internal();
int temp_external();
char * temp_format(int temp);

#endif // INCLUDED_TEMP

