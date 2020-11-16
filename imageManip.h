//Fanbo Xu; Ruixuan Zhai
//fxu13; rzhai2

#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include <stdio.h>

/* A function to swap colors
 */
int swap(Image * im);

/* two functions that change the 
 * brightness of a picture
 */
unsigned char helper_brightness(double value);
int bright(int input, Image * im);

/* invert the picture
 * subtract from 255
 */
int invert(Image * im);

/* change the photo to gray
 */
int gray(Image * im);

/* do the crop
 */
Image * crop(const Image * im, int left_col, int left_row, int right_col, int right_row);
/* A helper function for blur
 * create matrix
 */
double ** g_matrix(double input);

/* the blur function
 */
int blur(double input, Image * im);

/* the edge function
 */
int edges(double input1, int input2, Image * im);
#endif
