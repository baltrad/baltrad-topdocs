#include "example.h"


int query(SCAN *myscan) {
   int bytes_per_bin = 0;
   printf("This scan's elevation angle is %2.1f degrees\n", myscan->elev);   
   printf("This scan has %ld rays\n", myscan->nazim);
   printf("This scan has %ld bins per ray\n", myscan->nrang);
   printf("The bin length is %3.1f m\n", myscan->rscale);
   printf("The data buffer contains %ld bytes\n", myscan->bytes);
   bytes_per_bin = (int)((float)myscan->bytes/(float)(myscan->nazim*myscan->nrang));
   printf("%d byte(s) per bin\n", bytes_per_bin);

   return 0;
}


int map_ODIM_to_SCAN(PolarScan_t *odim_scan, SCAN *my_scan) {
   RaveDataType type=RaveDataType_UNDEFINED;
   PolarScanParam_t *dbzh=NULL;
   size_t bins=0;
   size_t bytes=0;

   /* RAVE represents angles in radians, use PROJ.4 to convert back to degrees */
   my_scan->elev = PolarScan_getElangle(odim_scan) * RAD_TO_DEG;

   my_scan->nazim = PolarScan_getNrays(odim_scan);
   my_scan->nrang = PolarScan_getNbins(odim_scan);
   my_scan->rscale = PolarScan_getRscale(odim_scan);
   bins = my_scan->nazim * my_scan->nrang;

   /* ODIM can contain several parameters for each scan. Get DBZH. */
   dbzh = PolarScan_getParameter(odim_scan, "DBZH");
   type = PolarScanParam_getDataType(dbzh);

   /* Based on data type, defined in rave_types.h, allocate memory.
      Works for CHAR, UCHAR, SHORT, INT, LONG, FLOAT, and DOUBLE*/
   if (type == RaveDataType_UCHAR) {
      bytes = bins * sizeof(unsigned char);
   } else if (type == RaveDataType_DOUBLE) {
      bytes = bins * sizeof(double);
   } /* ... etc ... */
   my_scan->bytes = bytes;
   my_scan->data = RAVE_MALLOC(bytes);
   memset(my_scan->data, 0, bytes);

   memcpy(my_scan->data, PolarScanParam_getData(dbzh), bytes);
   RAVE_OBJECT_RELEASE(dbzh);

   /* Normally, we want to include enough checks to give different return 
      codes if something goes wrong, but not in this simple case. */
   return 0;
}


int main(int argc,char *argv[]) {
   SCAN myscan;
   PolarScan_t *scan=NULL;
   int retcode = -1;

   RaveIO_t* raveio = RaveIO_open(argv[1]);  /* I/O container, open the file */

   /* Access the file's payload, checking that it's the anticipated type */
   if (RaveIO_getObjectType(raveio) == Rave_ObjectType_SCAN) {
      scan = (PolarScan_t*)RaveIO_getObject(raveio);
   }else{
      printf("Input file is not a polar scan. Giving up ...\n");
      RAVE_OBJECT_RELEASE(scan);
      return 1;
   }

   /* Map ODIM information to contributed structure */
   if (map_ODIM_to_SCAN(scan, &myscan)) {
      printf("Failed to map information. Giving up ...\n");
      RAVE_OBJECT_RELEASE(scan);
      RAVE_FREE(myscan.data);
      return 1;
   }

   /* This is where you add your functionality using your structures,
      preferably with more control depending on the return code. */
   retcode = query(&myscan);

   /* Mapping back to ODIM is done similarly, and the I/O container object
      is used to write ODIM_H5 output files. But we won't do that here. */

   RAVE_OBJECT_RELEASE(scan);
   RAVE_FREE(myscan.data);
}   
