// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These are RTTY functions that send RTTY using the attached
// Radiometrix NTX2

#include "rtty.h"
#include "tsip.h"

// These are the digital pins used to control the Radiometrix NTX2
// module.  TX0/TX1 are for the two tones used for RTTY.

#define TX0 8
#define TX1 3

// rtty_init: Set up the pins used to control the radio module
void rtty_init()
{
  pinMode( TX0, OUTPUT );
  pinMode( TX1, OUTPUT );
}

// The number of bits per character (7), number of start bits (1), number of stop bits (2)
// and the baud rate.

#define ASCII 7 
#define START 1
#define STOP 2
#define BAUD 50
#define INTER_BIT_DELAY (1000/BAUD)

// rtty_bit: sends a single bit via RTTY
void rtty_bit(int b) // Send 0 if b is 0, 1 if otherwise
{
  digitalWrite(TX0,(b>0)?HIGH:LOW);
  digitalWrite(TX1,(b>0)?LOW:HIGH);
  delay(INTER_BIT_DELAY);
}

// rtty_send: sends a null-terminated string via radio to the ground trackers
void rtty_send( char * s ) // The null-terminated string to transmit
{
  tsip_disable();
  
  char c;
  while ( c = *s++ ) {
    int i;
    for ( i = 0; i < START; ++i ) {
      rtty_bit(0);
    }
    
    int b;   
    for ( i = 0, b = 1; i < ASCII; ++i, b *= 2 ) {
      rtty_bit(c&b);
    }
    
    for ( i = 0; i < STOP; ++i ) {
      rtty_bit(1);
    }
  }
 
  // Note that when idling RTTY specifies that it be in the 'mark' state (or 1).  This
  // is achieved by the stop bits that were sent at the end of the last character. 
  
  tsip_enable();
}

