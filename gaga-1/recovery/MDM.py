# Dummy module that pretends to be the real MDM module inside the
# GM862
#
# Copyright (c) 2010 John Graham-Cumming

def send(m,d):
    return 1

def receive(d):
    return '+CSQ: 9,0'
