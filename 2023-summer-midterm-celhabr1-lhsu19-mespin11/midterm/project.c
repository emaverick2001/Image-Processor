// Maverick Espinosa mespin11
// Colton El-Habr celhabr1
// Leo Hsu lhsu19
/*****************************************************************************
 * Midterm Project - A program to run the image processing operations
 * Note: for naming convention, we try to follow Google C++ style guide:
 *       https://google.github.io/styleguide/cppguide.html
 * It is not compulsory, but you are highly encouraged to follow a convention.
 *
 * Summary: This file implements a program for image processing operations.
 *          Different operations take different input arguments. In general,
 *            ./project <input> <output> <operation name> [operation params]
 *          The program will return 0 and write an output file if successful.
 *          Otherwise, the below error codes should be returned:
 *            1: Wrong usage (i.e. mandatory arguments are not provided)
 *            2: Input file I/O error
 *            3: Output file I/O error
 *            4: The Input file cannot be read as a PPM file
 *            5: Unsupported image processing operations
 *            6: Incorrect number of arguments for the specified operation
 *            7: Invalid arguments for the specified operation
 *            8: Other errors
 *****************************************************************************/
#include <stdlib.h>  // c functions: atio
#include <string.h>  // strcmp

#include "img_processing.h"  // image modification functions
#include "ppm_io.h"          // PPM I/O header

// TODO: include requried headers for your projects.
// We recommend to put your image processing operations in
//  img_processing.h for decleartions and
//  img_processing.c for their defintions
// Then you should include the below header:
// #include "img_processing.h" // Image processing header

int main(int argc, char **argv) {
  // Opening the image and reading it out unchanged

  // for now just generalizing cmd line arg but will need to specify it for each
  // argc value ex argc = 5 for binarize, argc = 4 for grayscale
  int return_code = 0;
  if (argc > 8 || argc < 4) {
    printf(
        "Usage: %s [input filename_out=kitten.ppm] [output "
        "filename_out=kittentest.ppm] [modification type] [parameters "
        "associated with mod type]\n",
        argv[0]);
    return_code = 1;  // return 1 for wrong usage
  } 
  
  else {
    // initializes cmd line arguments
    char *filename_read = argv[1];      // kitten.ppm
    char *filename_out = argv[2];       // kittentest.ppm
    char *modification_type = argv[3];  // ex "binarize", "grayscale"
    int threshold_val = -1; 
    int top_col = 0;  // 50
    int top_row = 0;  // 50
    int bot_col = 0;  // 100
    int bot_row = 0;  // 100
    float seam_col = argc == 6 ? atof(argv[4]) : 0;
    float seam_row = argc == 6 ? atof(argv[5]) : 0;

    // open up img to read
    FILE *input = fopen(filename_read, "rb");
    if (!input) {
      printf("Couldn't open input file: %s\n", filename_read);
      return_code = 2;
    }

    // open up output file to store modified image in
    FILE *output = fopen(filename_out, "wb");
    if (!output) {
      printf("Couldn't open output file: %s\n", filename_out);
      return_code = 3;  // return 2 for other errors
    }

    // set image location to im
    Image *im = ReadPPM(input);
    if (im == NULL){
      return_code = 4; //file can't be read as ppm file  
    }

    // cmd arg menu
    if (argc == 4) {
      if (strcmp(modification_type, "grayscale") == 0) {
        return_code = grayscale(im);
      } else if (strcmp(modification_type, "gradient") == 0) {
        return_code = gradient(im);
      } else if (strcmp(modification_type, "transpose") == 0) {
        return_code = transpose(im);
      } else {
        return_code = 7;
      }
    } 

    else if (argc == 5) {
      if (strcmp(modification_type, "binarize") == 0) { // TODO: need to test if choose binarize but not 5 args -> return code == 6 -> should we do the menu in terms of the name of the process to account for this?
        if (atoi(argv[4]) != 0 || *argv[4] == 48){ 
          threshold_val = atoi(argv[4]);
          return_code = binarize(im, threshold_val);
        } else {
          return_code = 7;
        } 
      } else {
        return_code = 7; // 5 args but not binarize return code is 7
      }
    } 
    else if (argc == 6) {
      if (strcmp(modification_type, "seam") == 0) {
        return_code = seamCarve(im, seam_col, seam_row);
      } else {
        return_code = 7;// 6 args but not seam return code is 7
      }
    }     
     
    else if (argc == 7) {  // no func have 7 param
      return_code = 5;
    }

    else if (argc == 8) {
      if (strcmp(modification_type, "crop") == 0) {
        if (atoi(argv[4]) != 0 || *argv[4] == 48 || argv[5] != 0 || *argv[5] == 48 || argv[6] != 0 || *argv[6] == 48 || argv[7] != 0 || *argv[7] == 48){ 
          top_col = atoi(argv[4]);
          top_row = atoi(argv[5]);
          bot_col = atoi(argv[6]);
          bot_row = atoi(argv[7]);
          return_code = crop(im, top_col, top_row, bot_col, bot_row);
        } else {
          return_code = 7; // user enters char instead of int
        } 
      } else {
        return_code = 7; // 8 args but not binarize return code is 7
      }   
    }

    if (!return_code) {
      // write image to output file
      int num_pixels_written = WritePPM(output, im);
      fclose(input);
      printf("Image created with %d pixels.\n", num_pixels_written);

      // clean up!
      free(im->data);  // releases the pixel array
      free(im);        // releases the image itself
    } 
    else {
      free(im->data);  // releases the pixel array
      free(im);        // releases the image itself
    }
  }
  
  if (return_code) {
    printError(return_code, argv);
  }

  return return_code;

}
