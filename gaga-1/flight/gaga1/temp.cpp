// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These are functions for reading temperatures from the internal and
// external DS1821+ sensors using the OneWire library

#include "temp.h"
#include "ds1821.h"

// These are the pins where the sensors are attached.  Each sensor has a single pin.

#define INTERNAL 8
#define EXTERNAL 9

// temp_init: set up the sensors for use
void temp_init() 
{
  pinMode(INTERNAL, INPUT);
  digitalWrite(INTERNAL, LOW);
}

// temp_get: Read the temperature from a DS1821 and return it in tenths of degrees C
int temp_get( int pin ) // Arduino pin the DS1821 is attached to
{
  // The timing is very tight so disable interrupts during this action
  
  cli();
  
  // Tell the DS1821 to start a temperature conversion.  This can take up to 
  // 1s so have to loop reading the status register to see if bit 7 is set. 
  // When it is set the conversion has completed.
  
  ds1821_write_bits( 8, 0xEE, pin );
  
  for ( int i = 0; i < 100; ++i ) {
    delayMicroseconds(10000);
    
    ds1821_write_bits( 8, 0xAC, pin );
    int status = ds1821_read_bits( 8, pin );
    if ( status & 0x80 ) {
      break;
    }
  }

  // Now read the actual temperature value followed by information used
  // to calculate the tenths of degrees.  The exact calculation is
  // detailed in the DS1821 datasheet.
  
  ds1821_write_bits( 8, 0xAA, pin );
  int low = ds1821_read_bits( 8, pin );
   
  ds1821_write_bits( 8, 0xA0, pin );
  int remaining = ds1821_read_bits( 9, pin );

  ds1821_write_bits( 8, 0x41, pin );
  ds1821_write_bits( 8, 0xA0, pin );
  int slope = ds1821_read_bits( 9, pin );
   
  sei();

  // The temperature is negative if the top (7th) bit is set.  Convert to 
  // a negative integer.

  if ( low >= 128 ) {
   low -= 256;
  }   

  // See datasheet for explanation of this calculation.  Working here in tenths of
  // degrees since can't do floating point arithmetic.
  
  return 10 * low - 5 + 10 * ( slope - remaining ) / slope;
}

// temp_internal: retrieve the internal temperature of the capsule in tenths of
// degrees C
int temp_internal()
{
  return temp_get(INTERNAL);
}

// temp_external: retrieve the external temperature of the capsule in tenths of
// degrees C
int get_external_temp()
{
  return temp_get(EXTERNAL);
}



