//Fanbo Xu; Ruixuan Zhai
//fxu13; rzhai2

#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"
#include <string.h>
#include "imageManip.h"
#include <ctype.h>

int main(int argc, char * argv[]) {
  FILE * ppm_photo = fopen(argv[1], "rb");
  if (!ppm_photo) {
    printf("Specified input file could not be opened\n");
    return 2;
  }
  int input_name_length = strlen(argv[1]);
  int output_name_length = strlen(argv[2]);
  if ((input_name_length < 5) || (output_name_length < 5)){
    printf("Failed to supply input filename or output filename or both\n");
    return 1;
  }
  else {
    if ((argv[1][input_name_length - 4] != '.') || (argv[1][input_name_length - 3] != 'p') || (argv[1][input_name_length - 2] != 'p') || (argv[1][input_name_length - 1] != 'm')) {
      printf("Failed to supply input filename or output filename or both\n");
      return 1;
    }
    else if ((argv[2][output_name_length - 4] != '.') || (argv[2][output_name_length - 3] != 'p') || (argv[2][output_name_length - 2] != 'p') || (argv[2][output_name_length - 1] != 'm')) {
      printf("Failed to supply input filename or output filename or both\n");
      return 1;
    }
  }
  Image * image = read_ppm(ppm_photo);
  if (image == NULL) {
    printf("Specified input file is not a properly-formatted PPM file, or reading input somehow failes\n");
    return 3;
  }
  FILE * ppm_after = fopen(argv[2], "wb");
  if (!ppm_after) {
    printf("Specified output file could not be opened for writing, or writing output somehow fails\n");
    free(image->data);
    free(image);
    return 7;
  }
  fclose(ppm_photo);
  if (argc < 4) {
    printf("No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }
  int input = 0;
  //if the command is swap
  if (strcmp(argv[3], "swap") == 0) {
    //check if the argument number is right
    if (argc != 4) {
      printf("Incorrect number of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }
    //do the swap
    swap(image);
    //write the result
    write_ppm(ppm_after, image);
  }

  //If the command is bright
  else if (strcmp(argv[3], "bright") ==0) {
    //check if the argument number is right
    if (argc != 5) {
      free(image->data);
      free(image);
      fclose(ppm_after);
      printf("Incorrect number of argument\n");
      return 5;
    }
    //check if the argument (the last brightness number) is correct
    int length = strlen(argv[4]);
    if(!isdigit(argv[4][0])){
      if ((argv[4][0] != '-') && (argv[4][0] != '+')) {
	printf("The argument for changing brightness does not make sense\n");
	free(image->data);
	free(image);
	fclose(ppm_after);
	return 6;
	}
      else if (length == 1) {
	printf("The argument for changing brightness does not make sense\n");
	free(image->data);
	free(image);
	fclose(ppm_after);
	return 6;
      }
    }
    else if (length > 1) {
      for (int i = 1; i < length; ++i) {
	if (!isdigit(argv[4][i])) {
	  printf("The arugment for changing brightness does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
	}
      }
    }
    //change the input into int
    input = atoi(argv[4]);
    //do the brightness change
    bright(input, image);
    //write out the result
    write_ppm(ppm_after, image);
  }

  //If the command is invert
  else if (strcmp(argv[3], "invert") == 0) {
    //check if the input argument number is correct
    if (argc != 4) {
      printf("Incorrect nunber of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }
    //do the invert
    invert(image);
    //write out the result
    write_ppm(ppm_after, image);
  }

  //If the command is gray
  else if (strcmp(argv[3], "gray") == 0) {
    //check if the input argument number is correct
    if (argc != 4) {
      printf("Incorrect number of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }
    //do the gray
    gray(image);
    //write out the result
    write_ppm(ppm_after, image);
  }

  //If the command is crop
  else if (strcmp(argv[3], "crop") == 0) {
    //check if the input argument number is correct
    if (argc != 8) {
      printf("Incorrect number of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }

    //check if the input integers are correct
    int length = 0;
    int integers[4] = {0, 0, 0, 0};
    for (int k = 4; k < 8; ++k) {
      length = strlen(argv[k]);
      if (!isdigit(argv[k][0])) {
	if (argv[k][0] != '+') {
	  printf("The argument for crop does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
	}
	else if (length == 1){
	    printf("The argument for crop does not make sense\n");
	    free(image->data);
	    free(image);
	    fclose(ppm_after);
	    return 6;
	}
      }
      else {
	if (length > 1) {
	  for (int i = 1; i < length; ++i) {
	    if (!isdigit(argv[k][i])) {
	      printf("The argument for crop does not make sense\n");
	      free(image->data);
	      free(image);
	      fclose(ppm_after);
	      return 6;
	    }
	  }
	}
      }
      integers[k - 4] = atoi(argv[k]);
    }
      
    //do the crop
    Image * im = crop(image, integers[0], integers[1], integers[2], integers[3]);
    //write out the result
    write_ppm(ppm_after, im);
    free(im->data);
    free(im);
  }

  //If the command is blur
  else if (strcmp(argv[3], "blur") == 0) {
    int count = 0;
    //check if the number of argument is right
    if (argc != 5) {
      printf("Incorrect number of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }
    //check if the input is correct
    int length = strlen(argv[4]);
    if (!isdigit(argv[4][0])) {
      if ((argv[4][0] != '+') && (argv[4][0] != '.')) {
	  printf("The argument for blur does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
      }
      if (argv[4][0] == '.') {
	  count += 1;
      }
      if(length == 1) {
	  printf("The argument for blur does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
      }
    }
    else{
      if (length > 1) {
	for (int i = 1; i < length; ++i) {
	  if ((!isdigit(argv[4][i])) && (argv[4][i] != '.')) {
	    printf("The argument for blur does not make sense\n");
	    free(image->data);
	    free(image);
	    fclose(ppm_after);
	    return 6;
	  }
	  else if (argv[4][i] == '.') {
	    count += 1;
	    if (count > 1) {
	      printf("The argument for blur does not make sense\n");
	      free(image->data);
	      free(image);
	      fclose(ppm_after);
	      return 6;
	    }
	  }
	}
      }
    }
    double input = atof(argv[4]);
    //use the blur method
    blur(input, image);
    //write it out
    write_ppm(ppm_after, image);
    
  }

  //If the command is edge
  else if (strcmp(argv[3], "edges") == 0) {
    int count = 0;
    if (argc != 6) {
      printf("Incorrect number of argument\n");
      free(image->data);
      free(image);
      fclose(ppm_after);
      return 5;
    }
    int length = strlen(argv[4]);
    if (!isdigit(argv[4][0])) {
	if (argv[4][0] != '+') {
	  printf("The argument for edge does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
	}
	if (length == 1) {
	  printf("The argument for edge does not make sense\n");
	  free(image->data);
	  free(image);
	  fclose(ppm_after);
	  return 6;
	}
      }
      else{
	if (length > 1) {
	  for (int i = 1; i < length; ++i) {
	    if (!isdigit(argv[4][i]) && (argv[4][i] != '.')) {
		printf("The argument for edge does not make sense\n");
		free(image->data);
		free(image);
		fclose(ppm_after);
		return 6;
	    }
	    if (argv[4][i] == '.') {
	      count += 1;
	      if (count > 1) {
		printf("The argument for edge does not make sense\n");
		free(image->data);
		free(image);
		fclose(ppm_after);
		return 6;
	      }
	    } 
	       
	  }
	}
      }
      length = strlen(argv[5]);
      if (!isdigit(argv[5][0])) {
	  if (argv[5][0] != '+') {
	    printf("The argument for edge does not make sense\n");
	    free(image->data);
	    free(image);
	    fclose(ppm_after);
	    return 6;
	  }
	  if (length == 1) {
	    printf("The arugent for edge does not make sense\n");
	    free(image->data);
	    free(image);
	    fclose(ppm_after);
	    return 6;
	  }
      }
      else {
	if (length > 1) {
	  for (int i = 0; i < length; ++i) {
	    if (!isdigit(argv[5][i])) {
		printf("The argument for edge does not make sense\n");
		free(image->data);
		free(image);
		fclose(ppm_after);
		return 6;
	      }
	  }
	}
      }
      int input2 = atoi(argv[5]);
      double input1 = atof(argv[4]);
      edges(input1, input2, image);
      write_ppm(ppm_after, image);
  }
  else {
    printf("Operation name specified was invalid or no operation name was specified\n");
    free(image->data);
    free(image);
    fclose(ppm_after);
    return 4;
  }
  fclose(ppm_after);
  free(image->data);
  free(image);
  return 0;
}
