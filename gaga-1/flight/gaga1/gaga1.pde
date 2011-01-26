// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// This code performs the following:
//
// 1. Read temperature values from internal and external sensors
// 2. Read GPS position from Lassen IQ module
// 3. Transmit temperature and position as string using RTTY

// These are the digital pins used to control the Radiometrix NTX2
// module.  TX0/TX1 are for the two tones used for RTTY and EN is
// connected to the Enable pin to enable or disable the module

#define EN  6
#define TX0 8
#define TX1 7

// These are the pins used to read/write two the internal and
// external temperature sensors

#define TEMP_INT
#define TEMP_EXT

void setup()
{
  // Set up the pins used to control the radio module and switch
  // it on
  
  pinMode( EN,  OUTPUT );
  pinMode( TX0, OUTPUT );
  pinMode( TX1, OUTPUT );
  digitalWrite( EN, HIGH );
  
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


// ---------------------------------------------------------------------------------
// RTTY Code
//
// Code to send strings via RTTY.  The RTTY parameters are defined by constants
// below.
// ---------------------------------------------------------------------------------

// The number of bits per character (7), number of start bits (1), number of stop bits (2)
// and the baud rate.

#define ASCII 7 
#define START 1
#define STOP 2
#define BAUD 50
#define INTER_BIT_DELAY (1000/BAUD)

// rtty_send: sends a null-terminated string via radio to the ground trackers
void rtty_send( char * s ) // The null-terminated string to transmit
{
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
}

// rtty_bit: sends a single bit via RTTY
void rtty_bit(int b) // Send 0 if b is 0, 1 if otherwise
{
  digitalWrite(TX0,(b>0)?HIGH:LOW);
  digitalWrite(TX1,(b>0)?LOW:HIGH);
  delay(INTER_BIT_DELAY);
}

