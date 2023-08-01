// Maverick Espinosa mespin11
// Colton El-Habr celhabr1
// Leo Hsu lhsu19
/*****************************************************************************
 * Midterm Project - PPM file input/output header file
 * Note: for naming convention, we try to follow Google C++ style guide:
 *       https://google.github.io/styleguide/cppguide.html
 * It is not compulsory, but you are highly encouraged to follow a convention.
 * 
 * Summary: This file provides the helper functions for image modifications
 *****************************************************************************/
#ifndef MIDTERM_img_processing
#define MIDTERM_img_processing

#include <stdio.h> // c file type: FILE
#include "ppm_io.h"


/* grayscale
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * applies grayscale modification based on pixel rgb value
 * Return 0 if successful.
 */
int grayscale(Image *im);

/* binarize
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * converts grayscale image to black and white image
 * Return 0 if successful.
 */
int binarize(Image *im, int thresh);



/* crop
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * crops section of read image with user top left and bottom right coordinates
 * Return 0 if successful.
 */
int crop(Image *im,int top_col, int top_row, int bot_col, int bot_row);

/* transpose
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * transposes image along its diagonal
 * Return 0 if successful.
 */
int transpose(Image *im);

/* printError
 * Print error messages given return code
 */
void printError(const int return_code, char *argv[]);

/* gradient
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * applies gradient value to rgb color calues
 * Return 0 if successful.
 */
int gradient(Image *img);
#endif // MIDTERM_img_processing

int getGradientVals(Image *img, int return_ra[]);

int removeSeam(Image *img);

int returnMin(int a, int b, int c);

int seamCarve(Image *img, float col_size, float row_size);
