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
#include "time.h"

// This is only here so that the Arduino development environment will go and find the EEPROM
// library and include it in the program.  This is an ugly hack and shouldn't need to be here.

#include <EEPROM.h>

// Set by the call to fdr_init and contains the boot counter (offset into EEPROM where flight
// information is recorded).

int boot;

void setup()
{
    rtty_init();
    temp_init();
    boot = fdr_init();
    time_init();
}

// Calculate the XOR checksum (XOR of all characters that are not $
// in the string) and append as a two-character hex number.
void append_checksum( char * string ) // Pointer to string to be checksummed
{
  char * read = string;
  unsigned int checksum = 0;
  
  while ( *read ) {
    if ( *read != '$' ) {
      checksum ^= *read;
    }
    
    ++read;
  }
  
#define MAX_CHECKSUM_STRING 3
  char checksum_string[MAX_CHECKSUM_STRING+1];
  sprintf( checksum_string, "*%2.2X", checksum );
  strcat( string, checksum_string );
}

// The number of milliseconds between each report transmitted
#define INTER_REPORT_DELAY 5000

// loop:  called repeatedly by the Arduino.  All this does is send a status report via
// RTTY.
void loop()
{
  // The report format is as follows (fields are comma separated)
  //
  // Flight name: $$GAGA1
  // Seconds since launch
  // GPS time (HHmmss)
  // Latitude in dotted decimal format (DD.dddd)
  // Longitude in dotted decimal format (DD.dddd)
  // Altitude (in meters)
  // Internal temperature in C
  // External temperature in C
  // Boot counter from the FDR
  //
  // This is followed by a * and the checksum and a newline
  
  #define MAX_REPORT 128
  char report[MAX_REPORT+1] = "$$GAGA,";              // 7 chars

  strcat( report, time_format(time_elapsed()) );      // 6 chars

  strcat( report, temp_format(temp_internal()) );     // 4 chars
  strcat( report, temp_format(temp_external()) );     // 4 chars
  
#define MAX_BOOT_STRING 32
  char boot_string[MAX_BOOT_STRING];
  sprintf( boot_string, "%d,", boot );                // 6 chars
  strcat( report, boot_string );
  
  append_checksum( report );                          // 3 chars
  
  strcat( report, "\n" );                             // 1 char
                                                      // --------
                                                      // 31 chars

  rtty_send(report);
  
  delay(INTER_REPORT_DELAY);
}




