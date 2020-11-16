//Fanbo Xu; Ruixuan Zhai
//fxu13; rzhai2

#include <stdio.h>
#include "ppm_io.h"
#include "imageManip.h"
#include <stdlib.h>
#include <math.h>

/*Swap the color channels;
 *Copy each component's value to the next component
 */
int swap(Image * im) {
  int col = im->cols;
  int row = im->rows;
  char temp = 0;
  //iterate through all the pixels
  for (int i = 0; i < col * row; ++i) {
    temp = im->data[i].r;
    im->data[i].r = im->data[i].g;
    im->data[i].g = im->data[i].b;
    im->data[i].b = temp;
  }
  return col * row * 3; //return the number of pixels being swapped
}

/* helper method for adjusting brightness (control the pixel number)
 */
unsigned char helper_brightness(double value) {
  //iterate through all the pixels
  //if greater than 255 change to 255
  //if less than 0 change to 0
  if (value > 255.0) {
    value = 255.0;
  }
  else if (value < 0.0) {
    value = 0.0;
  }
  return (unsigned char) value;
}

/*This is the function that changes the brightness of the picture
 *based on the user input
 *It also checks if it goes over the range (0, 255)
 */
int bright(int input, Image * im) {
  int row = im->rows;
  int col = im->cols;
  int count = 0;

  //Change the pixel number
  for (int i = 0; i < row * col; ++i) {
    double temp = (double)(im->data[i].r);
    temp += input;
    im->data[i].r = helper_brightness(temp);
    temp = (double)(im->data[i].g);
    temp += input;
    im->data[i].g = helper_brightness(temp);
    temp = (double)(im->data[i].b);
    temp += input;
    im->data[i].b = helper_brightness(temp);
    count += 1;
  }
  return count;
}

/*This is the function that invert a picture
 *It subtracts each pixel value from 255
 *and put the result back to the pixels
 */
int invert(Image * im) {
  int row = im->rows;
  int col = im->cols;
  int count = 0;
  for (int i = 0; i < row * col; ++i) {
    im->data[i].r = 255 - im->data[i].r;
    im->data[i].g = 255 - im->data[i].g;
    im->data[i].b = 255 - im->data[i].b;
    count += 1;
  }
  return count;
}

/*This is the function that calculate the grayscale
 *and assign the same value to each pixel
 *which changes the picture to gray
 */
int gray(Image * im) {
  double intensity = 0;
  int count = 0;
  for (int i = 0; i < im->rows * im->cols; ++i) {
    intensity = 0.30 * im->data[i].r + 0.59 * im->data[i].g + 0.11 * im->data[i].b;
    im->data[i].r = intensity;
    im->data[i].g = intensity;
    im->data[i].b = intensity;
    count += 1;
  }
  return count;
}

/*This is the function that crop the picture base on the user input
 *The top left corner and the right lower corner are given
 */
Image * crop(const Image * im, int left_col, int left_row, int right_col, int right_row) {
  int col = right_col - left_col;
  int row = right_row - left_row;
  int count = im->cols * left_row + left_col;
  Image * new = (Image *)malloc(sizeof(Image));
  new->rows = row;
  new->cols = col;
  Pixel * pix = (Pixel *)malloc(sizeof(Pixel) * col * row);
  pix[0].r = im->data[count].r;
  pix[0].g = im->data[count].g;
  pix[0].b = im->data[count].b;
  for (int i = 1; i < row * col; ++i) {
    pix[i].r = im->data[i + count].r;
    pix[i].g = im->data[i + count].g;
    pix[i].b = im->data[i + count].b;
    if (i % col == col - 1) {
      count += (im->cols - col);
    }
  }
  new->data = pix;
  return new;
}
#define PI 3.141592653589
#define sq(a) ((a) * (a))
/*This is a helper function for blur
 *This help create marix for further uses
 */
double ** g_matrix(double input) {
  //calculate the size of the matrix
  int size = input * 10;
  if (size % 2 == 0) {
    size += 1;
  }
  double sum = 0.0;
  //malloc the matrix
  double ** matrix = malloc(sizeof(double *) * size);
  int center = size / 2;
  for (int i = 0; i < size; ++i) {
    matrix[i] = malloc(sizeof(double) * size);
  }
  //fill the matrix with doubles
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i][j] = (1.0 / (2.0 * PI * sq(input))) * exp( -(sq(i - center) + sq(j - center)) / (2 * sq(input)));
      sum += matrix[i][j];
    }
  }
  return matrix;
}

/*This is the function that blurs the picture
 */
int blur(double input, Image * im) {
  //The size of the filter matrix
  int size = input * 10;
  if (size % 2 ==0) {
    size += 1;
  }
  //malloc a new image to store the data
  Image * temp = (Image *) malloc(sizeof(Image));
  temp->rows = im->rows;
  temp->cols = im->cols;
  temp->data = malloc(sizeof(Pixel) * im->rows * im->cols);
  //count how many pixels are changed
  int count = 0;
  //get the gauss matrix from the function
  double ** matrix = g_matrix(input);
  //iterate through the pixels and store them into the temp
  for (int i = 0; i < temp->rows * temp->cols; ++i) {
    double sum_matrix = 0.0;
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    //iterate through the gauss matrix
    for (int j = 0; j < size; ++j) {
      for (int k = 0; k < size; ++k) {
	int distance_x = k - size / 2;
	int distance_y = j - size / 2;
	int x = i / temp->cols;
	int y = i % temp->cols;
	//check if outof the picture
	if (distance_x < 0) {
	  distance_x *= -1;
	  if (x < distance_x) {
	    continue;
	  }
	}
	else if (distance_x > 0) {
	  if (temp->rows - 1 - x < distance_x) {
	    continue;
	  }
	}
	if (distance_y < 0) {
	  distance_y *= -1;
	  if (y < distance_y) {
	    continue;
	  }
	}
	else if (distance_y > 0) {
	  if (temp->cols - 1 - y < distance_y) {
	    continue;
	  }
	}
	//calculate the filter matrix times the picture colors
	//add the sums
	red += matrix[k][j] * im->data[(j - size / 2 + y) + (k - size / 2 + x) * temp->cols].r;
	green += matrix[k][j] * im->data[(j - size / 2 + y) + (k - size / 2 + x) * temp->cols].g;
	blue += matrix[k][j] * im->data[(j - size / 2 + y) + (k - size / 2 + x) * temp->cols].b;
	sum_matrix += matrix[k][j];
      }
    }
    //put the correct colors into the pixels
    temp->data[i].r = red / sum_matrix;
    temp->data[i].g = green / sum_matrix;
    temp->data[i].b = blue / sum_matrix;
    count += 1;
  }
  free(im->data);
  im->data = temp->data;
  free(temp);
  for (int i = 0; i < size; ++i) {
    free(matrix[i]);
  }
  free(matrix);
  return count;
}

int edges(double input1, int input2, Image * im) {
  int count = 0;
  //use the gray function
  gray(im);
  //use the blur function
  blur(input1, im);
  Image * temp = (Image *)malloc(sizeof(Image));
  temp->cols = im->cols;
  temp->rows = im->rows;
  temp->data = (Pixel*)malloc(sizeof(Pixel) * im->cols * im->rows);
  for (int l = 0; l < im->cols * im->rows; ++l) {
    temp->data[l].r = im->data[l].r;
    temp->data[l].g = im->data[l].g;
    temp->data[l].b = im->data[l].b;
  }
  double gradient_x = 0.0;
  double gradient_y = 0.0;
  //change the intensity
  for (int i = 1; i < (im->rows - 1); ++i) {
    for (int j = 1; j < (im->cols - 1); ++j) {
      gradient_x = (im->data[j + 1 + i * im->cols].r - im->data[j - 1 + i * im->cols].r) / 2.0;
      gradient_y = (im->data[j + (i + 1) * im->cols].r - im->data[j + (i - 1) * im->cols].r) / 2.0;
      double result = sqrt(gradient_x * gradient_x + gradient_y * gradient_y);
      if (result > input2) {
	temp->data[j + i * temp->cols].r = 0;
      }
      else {
	temp->data[j + i * temp->cols].r = 255;
      }
      temp->data[j + i * temp->cols].g = temp->data[j + i * temp->cols].r;
      temp->data[j + i * temp->cols].b = temp->data[j + i * temp->cols].r;
      count += 1;
    }
  }
  //check based on the input threshold
  free(im->data);
  im->data = temp->data;
  free(temp);
  return count;
}
