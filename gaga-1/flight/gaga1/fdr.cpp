// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// These functions are used to store values in the EEPROM in the Arduino.  Since
// this memory is non-volatile we can read it even after a power loss.

#include "fdr.h"

// This ugliness includes the EEPROM library header file here where it is needed.  See
// similar ugliness in the gaga1.pde to ensure that the library itself is included.

#include <../../../../libraries/EEPROM/EEPROM.h>

// These defines specify memory locations in the EEPROM where values are stored

#define FDR_BOOT 0

#define FDR_BOTTOM (FDR_BOOT + 1)
#define FDR_ALT FDR_BOTTOM
#define FDR_INT (FDR_ALT + 2)
#define FDR_EXT (FDR_INT + 2)
#define FDR_TOP (FDR_EXT + 2)

// This is the offset inside the EEPROM where data is written.  See explanation
// in fdr_init()

int fdr_offset;

// fdr_read_word: reads a pair of bytes at mem and mem+1 and treats them
// as a word with the LSB in mem, MSB in mem+1
unsigned int fdr_read_word( int mem ) // Memory location to read
{
  mem += fdr_offset;
  
  unsigned int low  = EEPROM.read(mem);
  unsigned int high = EEPROM.read(mem+1);
  
  return low + 256 * high;
}

//fdr_write_word: write a pair of bytes a mem and mem+1 from a word.  LSB
// in mem and MSB in mem+1
void fdr_write_word( int mem,            // Memory location to write to
                     unsigned int word ) // Word to write
{
  mem += fdr_offset;
  
  unsigned int low = word % 256;
  unsigned int high = word / 256;
  
  EEPROM.write( mem,   low  );
  EEPROM.write( mem+1, high );
}

int fdr_read_int( int mem ) // Memory location to read
{
  unsigned int word = fdr_read_word( mem );
  int result;
  
  if ( word < 32768 ) {
    result = (int)word;
  } else {
    result = (int)word - 32768;
  }
  
  return result;
}

void fdr_write_int( int mem,  // Memory location to write to  
                    int val ) // Value to write
{
  unsigned int word;
  
  if ( val < 32768 ) {
    word = (unsigned int)val;
  } else {
    word = (unsigned int)(val + 32768);
  }
  
  fdr_write_word( mem, word );
}

// fdr_init: reset the initial values in the data recorder
void fdr_init()
{
  // This stores six bytes of information and the Arduino has 1K of EEPROM, so I can store 170
  // lots of information.  To achieve this byte 0 contains a 'boot counter' which is used to
  // determine a position in memory for the next storage round.  Each time fdr_init() is called
  // the boot counter is incremented.  
  //
  // The boot counter is used to calculate an offset into the EEPROM which the read/write functions
  // will use.
  // 
  // Since we might actually use more than six bytes the FDR_TOP and FDR_BOTTOM macros are used
  // to calculate how much will fit in memory.
  
  unsigned int boot = EEPROM.read( FDR_BOOT );
  
  unsigned int slots = 1023 / ( FDR_TOP - FDR_BOTTOM ) - 1;
  
  if ( boot >= slots ) {
    boot = 0;
  } else {
    boot++;
  }
  
  EEPROM.write( FDR_BOOT, boot );
 
  fdr_offset = boot * ( FDR_TOP - FDR_BOTTOM );
  
  fdr_write_word( FDR_ALT, 0x0000 );
  fdr_write_int(  FDR_INT, 0x7FFF );
  fdr_write_int(  FDR_EXT, 0x7FFF );  
}

// fdr_max_altitude: store the maximum altitude attained
void fdr_max_altitude( unsigned int alt )  // The altitude in m
{
  unsigned int altitude = fdr_read_word( FDR_ALT );
  
  if ( alt > altitude ) {
      fdr_write_word( FDR_ALT, alt );
  }
}

void fdr_min_temperature( int temp,  // The temperature in tenths of C
                          int mem )  // Memory location to store it in
{
  int t = fdr_read_int( mem );
  
  if ( temp < t ) {
     fdr_write_int( t, mem );
  }
}

// fdr_min_internal: store the minimum internal temperature
void fdr_min_internal( int temp )  // The temperature in tenths of C
{
  fdr_min_temperature( temp, FDR_INT );
}

// fdr_min_external: store the minimum external temperature
void fdr_min_external( int temp )  // The temperature in tenths of C
{
  fdr_min_temperature( temp, FDR_EXT );
}



