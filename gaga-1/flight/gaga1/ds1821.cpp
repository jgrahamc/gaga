// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Code to interface to the DS1821 temperature sensor

#include "ds1821.h"

// ds1821_drive_low: force the DS1821 pin low and into output mode
// ready for writing data to it
void ds1821_drive_low( int pin ) // Arduino pin the DS1821 is on
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

// ds1821_reset: resets the DS1821 ready for communication.
int ds1821_reset( int pin ) // Arduino pin the DS1821 is on
{
  ds1821_drive_low( pin );
  
  // The DS1821 needs 480us of low, then after 60us it will 
  // drive the bus low which can then be detected.
  
  delayMicroseconds( 480 );
  pinMode( pin, INPUT );
  delayMicroseconds( 65 );

  int bus = digitalRead(pin);
  delayMicroseconds( 415 ); 

  return ( bus == LOW );
}

// ds1821_read_bit: read a single bit coming from the DS1821
int ds1821_read_bit( int pin ) // Arduino pin the DS1821 is on
{
  ds1821_drive_low( pin );
  
  // The read slot is 60us.  Start by pushing the bus low and
  // wait 1us, then switch to input mode.  Data is valid for 15us
  // and read the state from the DS1821.  
  
  delayMicroseconds( 1 );
  digitalWrite( pin, HIGH );

  pinMode( pin, INPUT );
  delayMicroseconds( 5 );
  int value = ( digitalRead( pin ) == HIGH );
  
  // Calculated as 60 - 5 - 1 + 1 for recovery time.
  
  delayMicroseconds( 55 );
  return value;
}

// ds1821_write_bit: write a single bit to the DS1821
void ds1821_write_bit( int b,  // The bit to be written
             int pin ) // Arduino pin the DS1821 is on
{
  ds1821_drive_low( pin );

  // Total write time is 60us.  The DS1821 samples the
  // bus between 15us and 60us.
  
  delayMicroseconds( 15 );

  if ( b ) {
    digitalWrite( pin, HIGH );
    delayMicroseconds( 45 );
  } else {
    delayMicroseconds( 45 );
    digitalWrite( pin, HIGH );
  }
  
  // Recovery time before next operation
  delayMicroseconds( 1 );
}

// ds1821_read_bits: read a sequence of bits from the DS1821
int ds1821_read_bits( int bits, // Number of bits to read
            int pin ) // Arduino pin the DS1821 is on
{
  int value = 1 << (bits-1);
  int b = 0;
  
  while ( bits > 0 ) {
     b >>= 1;
     
     if ( ds1821_read_bit(pin) ) {
     b |= value;
     }
     
     --bits;
  }

  return b;
}

// ds1821_write_bits: write a number of bits to the DS1821
void ds1821_write_bits( int bits, // Number of bits to write
            int w,  // The number to write bits from
            int pin ) // Arduino pin the DS1821 is on
{
  ds1821_reset(pin);

  while ( bits > 0 ) {
    ds1821_write_bit( w & 1, pin );
    w >>= 1;
    --bits;
  }
}




