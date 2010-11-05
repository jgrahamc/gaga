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
#
# Conversions performed:
#
# utc, altitude, course and speed converted to float
# satellites converted to int
# longitude and latitude converted to decimal degrees

def get():
    p = GPS.getActualPosition()
    logger.log( 'gps2.get() -> %s' % p )
    a = p.strip().split(',')
    if len(a) == 11 and int(a[5]) == 3:
        global lastaltitude
        latd = int(a[1][0:2])
        latm = float(a[1][2:-1])
        latc = a[1][-1]
        lat = '%.4f%s' % ( latd + latm / 60, latc )
        lond = int(a[2][0:3])
        lonm = float(a[2][3:-1])
        lonc = a[2][-1]
        lon = '%.4f%s' % ( lond + lonm / 60, lonc )
        return { 'latitude'   : lat,
                 'longitude'  : lon,
                 'altitude'   : float(a[4]),
                 'utc'        : float(a[0]),
                 'course'     : float(a[6]),
                 'satellites' : int(a[10]),
                 'speed'      : float(a[7]),
                 'valid'      : 1
                 }
    else:
        return { 'valid': 0 }

# Gets the current antenna voltage in mV

def voltage():
    return GPS.getAntennaVoltage()


     
                 
