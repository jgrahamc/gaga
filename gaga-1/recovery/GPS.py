# Dummy module that pretends to be the real GPS module inside the
# GM862
#
# Copyright (c) 2010 John Graham-Cumming

import random
import time

gpsalt = 11.33
gpsup = 1
gpstime = time.time()

def getPowerOnOff():
    return random.randint(0,1)

def powerOnOff(a):
    return a

# Make up a position randomly but run through a 'realistic' set of
# altitudes up to 40km to test the main loop.  Sometimes fail to get a
# GPS lock.
#
# 002544.000,5238.3233N,00211.3308W,1.2,14.3,3,170.10,0.36,0.19,051110,08

def getActualPosition():
    utc = time.strftime('%H%M%S.000', time.gmtime())
    day = time.strftime('%d%m%y', time.gmtime())
    if random.randint(0,100) < 10:
        l = random.randint(0,1) * 2
        return '%s,,,,,%d,,,,%s,00' % ( utc, l, day )
    else:
        global gpsalt
        global gpsup
        global gpstime
        spd = random.randint(0,5000)/100
        if gpsup:
            gpsalt += (time.time() - gpstime) * random.randint(30,50)
        else:
            gpsalt -= (time.time() - gpstime) * random.randint(30,50)
        if gpsalt > 40000:
            gpsup = 0
        if gpsalt < 20:
            gpsalt = random.randint(100,200)/10
            spd = 0
        gpstime = time.time()
        cog = random.randint(0,36000)/100
        spk = spd * 0.539956
        sats = random.randint(1,8)
        return '%s,5238.%04dN,00211.%04dW,1.2,%.2f,3,%.2f,%.2f,%.2f,%s,%02d' % ( utc, random.randint(0,9999), random.randint(0,9999), gpsalt, cog, spd, spk, day, sats )

def getAntennaVoltage():
    return random.randint(0,3900)

