# Module to provide simple SMS sending functions
#
# Copyright (c) 2010 John Graham-Cumming

import at
import logger

# Set up SMS by turning on text mode using the AT+CMGF command

def init():
    at.cmd( 'AT+CMGF=1' )

# Determine whether we have network connectivity. Uses the AT+CSQ
# command to get the status of the GSM connection.

def can():
    r = at.cmd( 'AT+CSQ' )

    # Comman returns something like
    #
    # +CSQ: 9,0
    #
    # So first split on whitespace, then on the :

    w = r.split()
    if len(w) < 2:
        return 0

    m = w[1].split(',')
    if len(m) != 2:
        return 0
    else:
        return ( int(m[0]) != 99 ) and ( int(m[1]) != 99 )

# Send a message to a specified phone number

def send(phone, text):
    logger.log( 'sms.send(%s...,"%s")' % ( phone[0:4], text ) )
    if can():
        r = at.cmd( 'AT+CMGS="%s"' % phone )
        if r == '\r\n> ':
            r = at.raw( '%s\x1A' % text )
        else:
            r = at.raw( '\x1B' )
            logger.log( 'Failed to get SMS prompt' )
    else:
        logger.log( 'No GSM access for SMS send' )
