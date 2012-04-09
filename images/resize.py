#!/usr/bin/env python
# $Id$
# Author: Daniel Michelson
# Copyright: SMHI, 2012-
# $Log$
# History: 
#
"""

"""
import sys
import Image


def main(fstr):
    this = Image.open(fstr)
    width, height = this.size
    that = this.resize((int(width*(2./3)),int(height*(2./3))),
                       resample=Image.ANTIALIAS)
    that.save(fstr)


if __name__ == "__main__":
    import glob
#    flist = glob.glob('data_source*png')
#    flist = flist + glob.glob('*_conf.png')
#    flist.append('login_page.png')
#    flist = ["pgf_adapter.png"]
#    for fstr in flist:
    main(sys.argv[1])
