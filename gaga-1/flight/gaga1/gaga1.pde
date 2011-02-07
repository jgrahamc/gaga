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

void setup()
{
    rtty_init();
}

void loop()
{
    char test[255];
    sprintf( test, "GAGA-1 High Altitude Balloon Test Transmission on 434.650 @ 10mW" );
    rtty_send(test);
    
    delay(5000);
}

// ---------------------------------------------------------------------------------
// GPS Code
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// Temperature Code
// ---------------------------------------------------------------------------------



