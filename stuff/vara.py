#!/usr/bin/env python
## Example of using scripts to process data and chain algorithms in memory

import sys
import _raveio
import ropo_realtime
import rave_composite
import _transform
#import rave_ql


def process(input_file, output_file):

    # Read ODIM file and get its payload
    io_container = _raveio.open(input_file)
    polar_volume = io_container.object

    # Detect and remove non-precipitation echoes. This generator is the "lazy"
    # alternative, equivalent of using --lookup=True on the command line.
    cleaned_volume = ropo_realtime.generate(polar_volume)

    # Generate single-site Pseudo-CAPPI "composite"
    composite = rave_composite.generate([cleaned_volume], area="sevar_gmaps",
                                        quantity="DBZH",
                                        product="PCAPPI",
                                        prodpar=1000.0,
                                        method="NEAREST_RADAR")

    # Perform rudimentary gap-filling on the composite
    t = _transform.new()
    gap_filled = t.fillGap(composite)
    composite.setData(gap_filled.getData())

    # Display a quick-look window of the result
    #rave_ql.ql(composite.getData())

    # Recycle the I/O container and write output ODIM_H5 file
    io_container.object = composite
    io_container.filename = output_file
    io_container.save()


if __name__ == "__main__":
    # No validation of arguments
    process(sys.argv[1], sys.argv[2])
