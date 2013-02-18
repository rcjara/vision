/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
  Pixel *background;
  int rows, cols, colors;
  int bgrows, bgcols, bgcolors;
  long imagesize;
  long i;
  float threshold = 1.5;

  if(argc < 4) {
    printf("Usage: ppmtest <input file> <background file> <output file>\n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* read in the background image */
  background = readPPM(&bgrows, &bgcols, &bgcolors, argv[2]);
  if(!background) {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* mess with the image here  */
  for(i=0; i<imagesize; i++) {
    if (image[i].b > (threshold * image[i].r) && image[i].b > (threshold * image[i].g))
    {
      image[i] = background[i];
    }
  }

  /* write out the resulting image */
  writePPM(image, rows, cols, colors /* s/b 255 */, argv[3]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return(0);
}
