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

void setup()
{
    Serial.begin(9600);

    rtty_init();
    temp_init();
}

void loop()
{
  Serial.print( temp_internal() );
  Serial.print( "\n" );
  delay(5000);
}




