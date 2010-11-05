# Dummy module that pretends to be the real MOD module inside the
# GM862
#
# Copyright (c) 2010 John Graham-Cumming

import time

def sleep(a):
    time.sleep(a/10)

def secCounter():
    return 42

