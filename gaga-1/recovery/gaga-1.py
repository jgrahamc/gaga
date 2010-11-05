# GAGA Recovery Computer Control
#
# Copyright (c) 2010 John Graham-Cumming
#
# Performs the following:
# 
# 1. After launch transmits regular SMS messages giving location
#    until above specified altitude
#
# 2. Above altitude periodically checks altitude
#
# 3. Once below specified altitude once again sends SMS messages
#    periodically

import MOD

import sms
import gps2
import phone
import util

# Given a dictionary from the GPS reports the current location via SMS
# if it is valid.  Should be passed the position from gps2.get(). 
#
# This function also reports the current antenna voltage as a proxy
# for battery voltage and the current module internal temperature

def report_position(p):
    v = gps2.voltage()
    t = util.temperature()

    if p['valid']:
        send_sms( 'Position %s %s %f %f %f (%d, %d)' % ( p['latitude'],
                  p['longitude'], p['altitude'], p['course'], p['speed'], v,
                  t ) )
    else:
        send_sms( 'No GPS lock (%d, %d)' % ( v, t ) ) 

# Performs a state transition updating the global state variable and
# SMSing the new state

def set_state(s):
     global state
     state = s
     send_sms( 'Transition to state %s' % s )

# Wrapper for the sms.send command so tha the phone number need only
# be entered in one place.  Note that the phone number for SMS
# messages comes from the phone module which implements a single
# function, number(), that returns a string containing the phone
# number.

def send_sms(m):
    sms.send(phone.number(), m)

# Key configuration parameters are: altitude above which a launch is
# detected, altitude above which SMS messages are supressed and
# altitude below which we are back able to SMS.  All altitudes are in
# meters.

ascent_altitude   = 1000
flight_altitude   = 3500
recovery_altitude = 3000

# The recovery computer runs through a sequence of simple states that
# determine its behaviour.  It starts in the Launch state, transitions
# to the Ascent mode once above a preset altitude, then moves to
# Flight mode once too high for safe SMS usage.  Once below the safe
# altitude it transitions to Recovery mode.

state = ''
set_state( 'LAUNCH' )
sms.init()
gps2.init()

# The rules for the states are as follows:
#
#   Launch: get GPS position every 1 minute and SMS, check for
#   transition to Ascent mode
#
#   Ascent: get GPS position every 2 minute and SMS, check for 
#   transition to flight mode
#
#   Flight: get GPS position every 5 minutes and check for 
#   transition to Recovery mode
#
#   Recovery: get GPS position every 1 minute and SMS

while 1:
    delay = 1
    position = gps2.get()

    if state == 'LAUNCH':
        report_position(position)
        if position['valid'] and ( position['altitude'] > ascent_altitude ):
            set_state( 'ASCENT' )
    elif state == 'ASCENT':
        report_position(position)
        if position['valid'] and ( position['altitude'] > flight_altitude ):
            set_state( 'FLIGHT' )
        delay = 2
    elif state == 'FLIGHT':
        if position['valid'] and ( position['altitude'] < recovery_altitude ):
            set_state( 'RECOVERY' )
        delay = 5
    elif state == 'RECOVERY':
        report_position(position)
    
    MOD.sleep(delay * 600)
