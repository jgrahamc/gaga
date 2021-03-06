// GAGA-1 Flight Computer
//
// Copyright (c) 2011 John Graham-Cumming
//
// Functions to access an Lassen IQ module via its TSIP interface
// on the Arduino's serial connection

#include "tsip.h"

#include "SoftwareSerial.h"

// The GPS is attached to pin 6 TXA and pin 2 RXA
SoftwareSerial gps( 6, 2 );

// Standard Trimble definitions for the start of a packet and the end
#define DLE 0x10
#define ETX 0x03

// The Trimble packet functions that we are either send or receive
#define TSIP_COMMAND_SET_IO_OPTIONS 0x35
#define TSIP_REPORT_VELOCITY_ENU 0x56
#define TSIP_REPORT_LLA_POSITION 0x4A

#define BYTE unsigned char

// This union is used for all the values stored in the position structure
// as we will receive an IEEE 754 single precision number from the Lassen IQ
// module as 4 bytes and need to convert to a float.

union float32 {
  BYTE bytes[4];
  float value;
};  

// Stores the last known information received from the GPS giving
// the position and speed of the balloon.  They are retrieved via 
// tsip_get.

struct position last;

// tsip_send: send a command to the Lassen module.  This adds the 
// DLE and DLE/ETX and escapes any DLE inside the packet
void tsip_send( BYTE * packet,  // Packet data to send 
                int    length ) // Number of bytes in buffer
{
#define MAX_PACKET 255
  BYTE buffer[MAX_PACKET];
  
  int i = 0;
  int j = 0;
  buffer[i++] = DLE;
  
  while ( length > 0 ) {
    if ( packet[j] == DLE ) {
      buffer[i++] = DLE;
    }
    buffer[i++] = packet[j];
    ++j;
    --length;
  }
  
  buffer[i++] = DLE;
  buffer[i++] = ETX;
    
  i = 0;
  j += 3;
  while ( j > 0 ) {
    gps.write( buffer[i] );
    ++i;
    --j;
  }
}

// tsip_get: return the position structure containing the latest fix
struct position tsip_get()
{
  return last;
}

// tsip_init: set up initial parameters on the GPS, GPS locks start at once
void tsip_init()
{
  last.latitude  = 0;
  last.longitude = 0;
  last.altitude  = 0;
  last.vertical  = 0;
  last.fix       = 0;
  
  tsip_enable();
  
  // Tell the GPS how to report GPS position and time information.  This will come in
  // automatically and be handled by tsip_handle below.
  //
  // LLA indicates that we will get automatic latitude, longitude and altitude
  // ENU means that we will get east, north, up velocity reports
  // UTC means that we want UTC rather than GPS time
  
#define IO_OPTIONS_LENGTH 5
#define LLA 0x01
#define ENU 0x02
#define UTC 0x01
  BYTE io_options[IO_OPTIONS_LENGTH] = { TSIP_COMMAND_SET_IO_OPTIONS, LLA, ENU, UTC, 0 };
  tsip_send( io_options, IO_OPTIONS_LENGTH );
}

// Helper to convert from a single precision value returned by the GPS to an Arduino float

#define COPY_SINGLE(_a,_b)            \
  {                                   \
      union float32 temp;             \
      temp.bytes[3] = packet[_b];     \
      temp.bytes[2] = packet[_b+1];   \
      temp.bytes[1] = packet[_b+2];   \
      temp.bytes[0] = packet[_b+3];   \
      last._a = temp.value;           \
  }

// tsip_position: handle the reported GPS position
void tsip_position( BYTE * packet,  // Pointer to the data inside an 0x4A packet
                    int    length ) // Length of data in the data portion
{
  // The returned format is:
  //
  // 00-03 Single precision latitude in radians (- = south)
  // 04-07 Single precision longitude in radians (- = west)
  // 08-11 Single precision altitude in meters
  // 12-15 Clock bias (ignored)
  // 16-19 UTC time of fix
  
  if ( length < 20 ) {
    return;
  }
  
  COPY_SINGLE(latitude,  0)
  COPY_SINGLE(longitude, 4)
  COPY_SINGLE(altitude,  8)
  COPY_SINGLE(fix,      16)
}

// tsip_velocity: handle the reported GPS velocity
void tsip_velocity( BYTE * packet,  // Pointer to the data inside an 0x56 packet
                    int    length )  // Length of data in the data portion
{
  // The returned format is:
  //
  // 00-03 Single precision easterly velocity meters/second (- = west)
  // 04-07 Single precision northerly velocity meters/second (- = south)
  // 08-11 Single precision vertical velocity meters/second (- = down)
  // 12-15 Clock bias (ignored)
  // 16-19 UTC time of fix
 
  if ( length < 20 ) {
    return;
  } 
  
  COPY_SINGLE(vertical, 8)
  COPY_SINGLE(fix,     16)
}


// tsip_handle: handle a single packet received from the module
void tsip_packet( BYTE * packet,  // The packet data
                  int    length ) // Number of bytes in packet
{
  BYTE function = packet[0];
    
  switch ( function ) {
    case TSIP_REPORT_LLA_POSITION:
      tsip_position( packet+1, length-1 );
      break;
      
    case TSIP_REPORT_VELOCITY_ENU:
      tsip_velocity( packet+1, length-1 );
      break;
      
    default:
      break;
  }
}

// tsip_handle: flush data from the serial buffer and detect TSIP packets and
// handle the ones we are interested in
void tsip_handle()
{
  while ( gps.available() ) {
     BYTE b = gps.read();
     
     if ( b == DLE ) {
       BYTE n = gps.peek();
       
       // This happens if we are escaping DLE or have ETX inside a packet.  In that
       // case we haven't found the start of a packet and so we need to move on
       // to the next real DLE
       
       if ( ( n == DLE ) || ( n == ETX ) ) {
         gps.read();
       } else {
         
         // Reach here when we've found a valid DLE and removed it from the 
         // serial buffer.  Next job is to read the rest of the packet from the
         // buffer (up to an unescaped ETX) and pass that to the packet handler
         
         BYTE packet[MAX_PACKET];  // TSIP documentation specifies 255 byte max
         int i = 0;
   
         while ( ( gps.available() ) && ( i < MAX_PACKET ) ) {
           b = gps.read();
           if ( b == DLE ) {
             if ( !gps.available() ) {
               break;
             } else {
               b = gps.read();
                 
               if ( b == ETX ) {
                 tsip_packet( packet, i );
                 break;
               }
             }
           }
           
           packet[i] = b;
           ++i;
         }
       }
     }
  }
}

// tsip_enable: start receiving packets from the Lassen IQ
void tsip_enable()
{
  gps.begin(9600);
}

// tsip_disable: stop receiving packets from the Lassen IQ.  This prevents
// any interrupts generated by the module from happening.
void tsip_disable()
{
  gps.end();
}

