// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// This code performs the following:
//
// 1. Read temperature values from internal and external sensors
// 2. Read GPS position from Lassen IQ module
// 3. Transmit temperature and position as string using RTTY

#include "rtty.h"
#include "temp.h"
#include "fdr.h"

// This is only here so that the Arduino development environment will go and find the EEPROM
// library and include it in the program.  This is an ugly hack and shouldn't need to be here.

#include <EEPROM.h>

void setup()
{
    Serial.begin(9600);

    rtty_init();
    temp_init();
    fdr_init();
}

void loop()
{
  Serial.print( temp_internal() );
  Serial.print( "\n" );
  delay(5000);
}




