# Module to provide simplified access to GPS functions
#
# Copyright (c) 2010 John Graham-Cumming

import GPS

import logger

# Ensure that the GPS module is enabled

def init():
    if GPS.getPowerOnOff() == 0:
        GPS.powerOnOff(1)

# Gets the current position (lat, long), bearing and altitude of the
# module.  Return a dictionary containing the information.
#
# The dictionary has latitude, longitude and altitude as expected.
# The utc is the UTC time from the GPS.  course is the cog, satellites
# is the nsat value. speed is the spkm (km/h) and valid indicates
# whether the GPS has a 3D fix.

def get():
    p = GPS.getActualPosition()
    logger.log( 'gps2.get() -> %s' % p )
    a = p.strip().split(',')
    if len(a) == 11 and int(a[5]) == 3:
        altitude = int(a[4].split('.')[0])
        return { 'latitude'   : a[1],
                 'longitude'  : a[2],
                 'altitude'   : altitude,
                 'utc'        : a[0],
                 'course'     : a[6],
                 'satellites' : a[10],
                 'speed'      : a[7],
                 'valid'      : 1
                 }
    else:
        return { 'valid': 0 }

# Gets the current antenna voltage in mV

def voltage():
    return GPS.getAntennaVoltage()


     
                 
