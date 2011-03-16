# Module to provide some wrappers for AT commands
#
# Copyright (c) 2010 John Graham-Cumming

import MOD

import at
import logger

epoch = -1

# Retrieves the internal temperature of the module and returns it,
# returns -1000 if something goes wrong

def temperature():
    t = at.cmd( 'AT#TEMPMON=1' )

    # Command returns something like:
    #
    # #TEMPMEAS: 0,28
    #
    # OK
    #
    # So split on whitespace first to isolate the temperate 0,28 and
    # then split on comma to get the temperature

    w = t.split()
    if len(w) < 2:
        logger.log( "Temperature read returned %s" % t )
        return -1000
  
    m = w[1].split(',')
    if len(m) != 2:
        logger.log( "Temperature read returned %s" % t )
        return -1000
    else:
        return int(m[1])

# Gets the time since start in seconds
# form T+nnn.  

def elapsed():
    t = MOD.secCounter()

    global epoch
    if epoch < 0:
        epoch = t

    t = t - epoch

    return t

# Gets the time since start in seconds and returns a string of the
# form T+nnn.  

def timestamp():
    return "T+%d" % elapsed()

