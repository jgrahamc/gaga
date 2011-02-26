// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These are functions for reading temperatures from the internal and
// external DS1821+ sensors using the OneWire library

#include "temp.h"
#include "ds1821.h"
#include "fdr.h"

// These are the pins where the sensors are attached.  Each sensor has a single pin.

#define INTERNAL 12
#define EXTERNAL 11

// temp_init: set up the sensors for use
void temp_init() 
{
  pinMode(INTERNAL, INPUT);
  digitalWrite(INTERNAL, LOW);
  
  pinMode(EXTERNAL, INPUT);
  digitalWrite(EXTERNAL, LOW);
}

// temp_get: Read the temperature from a DS1821 and return it in tenths of degrees C
// If it returns a value near absolute zero then this indicates that an error occurred
// while reading the temperature sensor.
int temp_get( int pin ) // Arduino pin the DS1821 is attached to
{
  // The timing is very tight so disable interrupts during this action
  
  cli();
  
  // Tell the DS1821 to start a temperature conversion.  This can take up to 
  // 1s so have to loop reading the status register to see if bit 7 is set. 
  // When it is set the conversion has completed.
  
  if ( !ds1821_write_bits( 8, 0xEE, pin ) ) {
    sei();
    return -2730;
  }
  
  int status = 0;
  for ( int i = 0; i < 100; ++i ) {
    delayMicroseconds(10000);
    
    if ( !ds1821_write_bits( 8, 0xAC, pin ) ) {
        sei();
        return -2731;
    }
    
    status = ds1821_read_bits( 8, pin );
    if ( status & 0x80 ) {
      break;
    }
  }
  
  if ( ( status & 0x80 ) == 0 ) {
    sei();
    return -2732;
  }

  // Now read the actual temperature value followed by information used
  // to calculate the tenths of degrees.  The exact calculation is
  // detailed in the DS1821 datasheet.
  
  if ( !ds1821_write_bits( 8, 0xAA, pin ) ) {
     sei();
     return -2733;
  }
  
  int low = ds1821_read_bits( 8, pin );
   
  if ( !ds1821_write_bits( 8, 0xA0, pin ) ) {
    sei();
    return -2734;
  }
  int remaining = ds1821_read_bits( 9, pin );

  if ( !ds1821_write_bits( 8, 0x41, pin ) ) {
    sei();
    return -2735;
  }
  if ( !ds1821_write_bits( 8, 0xA0, pin ) ) {
    sei();
    return -2736;
  }
  int slope = ds1821_read_bits( 9, pin );
   
  sei();

  // The temperature is negative if the top (7th) bit is set.  Convert to 
  // a negative integer.

  if ( low >= 128 ) {
   low -= 256;
  }   

  // If the slope value were zero then something has gone very wrong so fail
  // by return absolute zero
  
  if ( slope == 0 ) {
      return -2737;
  }

  // See datasheet for explanation of this calculation.  Working here in tenths of
  // degrees since can't do floating point arithmetic.
  
  return 10 * low - 5 + 10 * ( slope - remaining ) / slope;
}

// temp_internal: retrieve the internal temperature of the capsule in tenths of
// degrees C
int temp_internal()
{
  int t = temp_get(INTERNAL);
  
  if ( t > -2730 ) {
    fdr_min_internal( t );
  }

  return t;
}

// temp_external: retrieve the external temperature of the capsule in tenths of
// degrees C
int temp_external()
{
  int t = temp_get(EXTERNAL);
  
  if ( t > -2730 ) {
    fdr_min_external( t );
  }
  
  return t;
}

// temp_format: format a temperature returned by temp_internal or temp_external.
// This function understands the error temperatures returned by those functions.
char * temp_format(int temp) // Temperature returned by call to temp_in/external
{
  #define MAX_TEMP_STRING 32
  static char temp_string[MAX_TEMP_STRING+1];

  // The error messages are signalled by numbers below -273.0C.  The temperatures
  // are in tenths of degrees.

  if ( temp <= -2370 ) {
    int error = temp + 2730;
    sprintf( temp_string, "Error: %d,", -error );
  } else {
    int tenths = abs(temp % 10);
    int cents = temp/10;
    sprintf( temp_string, "%d.%d,", cents, tenths );
  }
  
  return temp_string;
}


