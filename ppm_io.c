//Fanbo Xu; Ruixuan Zhai
//fxu13; rzhai2
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <assert.h>
#include <string.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <ctype.h>



/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp);
  
  //create a image and check if it is successfully malloced
  Image * im = (Image *)malloc(sizeof(Image));
  if (!im) {
    fprintf(stderr, "Image allocation failed\n");
    return NULL;
  }
  
  // check the P6 head and read it in
  char head[3];
  for (int i = 0; i < 3; ++i) {
    head[i] = fgetc(fp);
  }
  if ((head[0] != 'P')||(head[1] != '6')||(head[2] != '\n')) {
    printf("Not a properly-formatted PPM file(head)\n");
    return NULL;
  }
  
  // check if there is comment under P6
  char check = 0;//to check if a new line starts with #
  char temp = 0;//to check if a char is the new line indicater
  check = fgetc(fp);
  if (!isdigit(check)){
    if (check == '#') {
      // if there is comment, go through it
      while(check == '#') {
	temp = fgetc(fp);
	if (temp == '\n') {
	  check = fgetc(fp);
	}
      }
      //end of the comment
      if (!isdigit(check)) {
	printf("Not a properly-formatted PPM file(comment)\n");
	return NULL;
      }
      else {
	fseek(fp, -1, SEEK_CUR);
      }
    }
    else {
      printf("Not a properly-formatted PPM file(comment)\n");
      return NULL;
    }
  }
  else {
    fseek(fp, -1, SEEK_CUR);
  }

  //check if exists following two numbers, and
  //put the corresponding row and coloum in 
  int col = 0;
  int row = 0;
  if (fscanf(fp, "%d %d", &col, &row) != 2) {
    printf("Not a properly-formatted PPM file(col and row)\n");
    return NULL;
  }
  else {
    if ((col >= 0) && (row >= 0)) {
      im->cols = col;
      im->rows = row;
    }
    else {
      printf("The number of row or collumn does not make sense\n");
      return NULL;
    }
  }

  //create pixels
  Pixel * pix = malloc(sizeof(Pixel) * col * row);
  if (!pix) {
    printf("Pixel array allocation failed\n");
    return NULL;
  }
  im->data = pix;
  
  //check if the following number is 255
  int color = 0;
  if (fscanf(fp, " %d\n", &color) != 1) {
    printf("Not a properly-formatted PPM file(color)\n");
    return NULL;
  }
  else {
    if (color != 255) {
      printf("Not a properly-formatted PPM file(color)\n");
      return NULL;
    }
  }

  //check pixels and put correct colors into the pixels
  if ((int)(fread(pix, sizeof(char), 3 * row * col, fp)) != 3 * row * col){
	printf("Not a priperly-formatted PPM file(pixels)\n");
	return NULL;
  }
  return im;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
    return 7;
  }

  return num_pixels_written;
}

