#!/usr/bin/env python
# $Id$
# Author: Daniel Michelson
# Copyright: SMHI, 2010-
# $Log$
# History: 
#
"""

"""
import sys

def main(**args):
    for key in args:
        print "%s: %s" % (key, args[key])


if __name__ == "__main__":
    main(areaid="swevar_gmaps", quantity="DBZH", product='PCAPPI',
         prodpar=1000.0, 
         gain=0.5, offset=-32.0, date='20120122', time='1123130',
         method='nearest', anomaly_qc="ropo,beamb")
