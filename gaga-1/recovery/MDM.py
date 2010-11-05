# Dummy module that pretends to be the real MDM module inside the
# GM862
#
# Copyright (c) 2010 John Graham-Cumming

import random

mdmreply = ''

# Emulates a variety of commands that are actually used in the
# recovery computer returning different results each time and
# simulating various errors

def send(m,d):
    global mdmreply
    if m == 'AT#TEMPMON=1\r':
        mdmreply = '#TEMPMEAS: 0,%d\r' % random.randint(-55,50)
    elif m == 'AT+CSQ\r':
        a = random.randint(0,31)
        b = random.randint(0,31)
        if random.randint(0,100) < 5:
            a = 99
        if random.randint(0,100) < 5:
            b = 99
        mdmreply = '+CSQ: %d,%d\r' % ( a, b )
    elif m == 'AT+CMGF=1\r':
        mdmreply = 'OK\r'
    elif m[0:9] == 'AT+CMGS="':
        if random.randint(0,100) < 10:
            mdmreply = ''
        else:
            mdmreply = '\r\n> '
    else:
        mdmreply = 'OK\r'

def receive(d):
    return mdmreply
