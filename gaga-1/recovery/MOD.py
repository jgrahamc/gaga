# Dummy module that pretends to be the real MOD module inside the
# GM862
#
# Copyright (c) 2010 John Graham-Cumming

import time
import calendar

# Emulates the MOD.sleep command which uses tenths of seconds.  The
# Python time.sleep uses seconds.

def sleep(a):
    time.sleep(a/10)

# Get the number of seconds since January 1, 2000

def secCounter():
    return int(time.time() - calendar.timegm(time.strptime('1 Jan 2000', '%d %b %Y')))

