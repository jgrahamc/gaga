# Module to provide some wrappers for AT commands
#
# Copyright (c) 2010 John Graham-Cumming

import at

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
        return -1000
  
    m = w[1].split(',')
    if len(m) != 2:
        return -1000
    else:
        return int(m[1])
