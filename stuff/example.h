#ifndef EXAMPLE_H
#define EXAMPLE_H
#include <stdio.h>
#include <string.h>

#include "rave_io.h"
#include "polarscan.h"
#include "polarscanparam.h"
#include "rave_types.h"
#include "rave_alloc.h"

/* Simplified structure */
struct scan {
   double elev;           /* Elevation of scan in deg. */
   long nrang;            /* Number of range bins in scan. */
   long nazim;            /* Number of azimuth rays in scan. */
   double rscale;         /* Size of range bins in scan in km. */
   unsigned char *data;   /* Data buffer */
   size_t bytes;          /* Data buffer size */
};
typedef struct scan SCAN;

#endif
