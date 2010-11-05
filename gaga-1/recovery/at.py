# Module to provide writing to the modem command interface
#
# Copyright (c) 2010 John Graham-Cumming

import MDM

import logger

# Sends a command to the modem, waits specified period for response
# and returns response.  If no response returns null string.  The text
# of the command does not need a terminator.  This will be added.  The
# default wait time is 5 seconds.

def cmd(text, wait=5):
    return raw( "%s\r" % text, wait )

# Similar to cmd but doesn't append the CR at the end and so can be
# used to send raw strings to the modem

def raw(text, wait=5):
    MDM.send( text, 0 )
    return MDM.receive( wait * 10 )




