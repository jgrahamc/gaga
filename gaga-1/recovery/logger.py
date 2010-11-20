# Module to provide simple logging to file and serial
#
# Copyright (c) 2010 John Graham-Cumming

import SER

import util

log_file = 'gaga-1.log'

# Writes a message to the log file and sends it to the serial port.
# In flight the serial port will do nothing and the log file will keep
# a record of what happened.  In debugging the serial port is written
# to.

def log(m):
    t = util.timestamp()
    d = '%s: %s\r' % (t, m)
    SER.send(d)
    try:
        f = open( log_file, 'a' )
        f.write(d)
        f.close()
    except:
        return
