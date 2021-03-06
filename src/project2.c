/*
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppmIO.h"
#include "vision.h"

#define USECPP 0
#define intensities 255
#define FOREGROUND 255
#define BACKGROUND 0

void grow(unsigned char *mask, unsigned char *grown, int rows, int cols);
void shrink(unsigned char *mask, unsigned char *shrunk, int rows, int cols);
void medianify(unsigned char *mask, unsigned char *median, int rows, int cols);

int main(int argc, char *argv[]) {

  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;
  int gain;
  unsigned char *mask = NULL;
  unsigned char *grown = NULL;
  unsigned char *shrunk = NULL;
  unsigned char *median = NULL;
  char *inputfile;
  char *maskfile;
  char *growfile;
  char *shrinkfile;
  char *medianfile;

  if(argc != 2) {
    printf("Usage: ./project2 <input file>\n");
    exit(-1);
  }

  inputfile = argv[1];
  maskfile = "masked.pgm";
  growfile = "grown.pgm";
  gain = 2;
  shrinkfile = "shrunk.pgm";
  medianfile = "median.pgm";

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, inputfile);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", inputfile);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;
  mask = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);
  grown = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);
  shrunk = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);
  median = (unsigned char*)malloc(sizeof(unsigned char) * imagesize);

  /* mask the image */
  for(i=0; i<imagesize; i++) {
    if (image[i].r > (gain * image[i].g)) {
      mask[i] = BACKGROUND;
    }
    else {
      mask[i] = FOREGROUND;
    }
  } 

  /* manipulate the images */
  grow(mask, grown, rows, cols);
  shrink(mask, shrunk, rows, cols);
  medianify(mask, median, rows, cols);

  /* write out the files */
  writePGM(mask, rows, cols, intensities, maskfile);
  writePGM(grown, rows, cols, intensities, growfile);
  writePGM(shrunk, rows, cols, intensities, shrinkfile);
  writePGM(median, rows, cols, intensities, medianfile);

  /* free the image memory */
  free(image);
  free(mask);
  free(grown);
  free(shrunk);
  free(median);

  return(0);
}

/* Median filter: pixel goes with a majority of neighboring pixel */
void medianify(unsigned char *mask, unsigned char *median, int rows, int cols) {

  long imagesize, i, neighbor;
  int x, y, j, k, total;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){ /* for each pixel */

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      median[i] = mask[i];
    } 
    
    else {

      x = i % cols;
      y = i / cols;

      total = 0;

      /* look at 8 nearest neighbors. Average wins, i is tiebreaker */
      for (j = -1; j <= 1; ++j) {
        for(k = -1; k <= 1; ++k) {

          neighbor = ( (x+j) + (cols*(y+k)) );

          if (mask[neighbor] == FOREGROUND) {
            ++total;
          }
          
          if (total > 4) {
            median[i] = FOREGROUND;
          }
          else {
            median[i] = BACKGROUND;
          }

        }
      }
    }  
  }
}

/* turn off any foreground pixel with a background pixel neighbor (8-connected) */
void shrink(unsigned char *mask, unsigned char *shrunk, int rows, int cols) {

  long imagesize, i, neighbor;
  int x, y, j, k;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      shrunk[i] = mask[i];
    } 
    /* ignore if background */
    else if (mask[i] == BACKGROUND) {
      shrunk[i] = 0;
    }
    /* if foreground, apply shrinking algorithm */
    else if (mask[i] == FOREGROUND) {
      x = i % cols;
      y = i / cols;

      /* look at 8 nearest neighbors. If any are background, turn pixel off */
      for (j = -1; j <= 1; ++j) {
        for(k = -1; k <= 1; ++k) {

          neighbor = ( (x+j) + (cols*(y+k)) );

          if (mask[neighbor] == BACKGROUND) {
            shrunk[i] = BACKGROUND;
            break; /* out of first loop */
          }
          else {
            shrunk[i] = FOREGROUND;
          }
          
        }
        if (shrunk[i] == BACKGROUND) break; /* out of second loop */
      }
    }  
    
    else {
      shrunk[i] = mask[i];
    }
  }
}

/* turn on any background pixel with a foreground pixel neighbor (8-connected) */
void grow(unsigned char *mask, unsigned char *grown, int rows, int cols) {

  long imagesize, i, neighbor;
  int x, y, j, k;

  imagesize = (long)rows * (long)cols;

  for (i=0; i < imagesize; ++i){

    /* don't process the 1 pixel layer around the whole image */
    if ((i < (cols)) || i > (imagesize - cols) || (i % cols) == 0 || (i % cols) == (cols-1)) {
      grown[i] = mask[i];
    } 
    /* ignore if foreground */
    else if (mask[i] == FOREGROUND) {
      grown[i] = mask[i];
    }
    /* if background, apply growing algorithm */
    else if (mask[i] == BACKGROUND) {
      x = i % cols;
      y = i / cols;

      /* look at 8 nearest neighbors. If any are foreground, turn pixel on */
      for (j = -1; j <= 1; ++j) {
        for(k = -1; k <= 1; ++k) {

          neighbor = ( (x+j) + (cols*(y+k)) );

          if (mask[neighbor] == FOREGROUND) {
            grown[i] = FOREGROUND;
          }
          else {
            
          }
        }
      }
    }  
    
    else {
      grown[i] = mask[i];
    }
  }
  
}




















