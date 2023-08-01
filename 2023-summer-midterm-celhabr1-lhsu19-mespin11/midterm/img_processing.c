// Maverick Espinosa mespin11
// Colton El-Habr celhabr1
// Leo Hsu lhsu19
/*****************************************************************************
 * Midterm Project - PPM file input/output header file
 * Note: for naming convention, we try to follow Google C++ style guide:
 *       https://google.github.io/styleguide/cppguide.html
 * It is not compulsory, but you are highly encouraged to follow a convention.
 *
 * Summary: This file implements the utility functions to read/write PPM file
 *****************************************************************************/
#include "img_processing.h"

#include <assert.h>  // c functions: assert
#include <stdlib.h>  // c functions: malloc, free
#include <math.h>    // c functions: fabs

#include "ppm_io.h"  // PPM I/O header

/* grayscale
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * applies grayscale modification based on pixel rgb value
 */
int grayscale(Image *im) {
  
  
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      
    unsigned char gray = 0;
    gray = 0.3 * (im->data)[i* im->cols +j].r + 0.59 * (im->data[i* im->cols +j]).g +
    0.11 * (im->data[i* im->cols +j]).b;
    Pixel gray_pixel = {gray,gray,gray};
    im->data[i* im->cols +j] = gray_pixel;
    }
  } 
  return 0;
}

/* binarize
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * converts grayscale image to black and white image
 */
int binarize(Image *im, int thresh) {
  if (!thresh){
        return 6;
      }

  if (thresh < 0 || thresh > 255){
        return 7;
      }
  grayscale(im);
  Pixel black = {0,0,0};
  Pixel white = {255,255,255};
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      
    unsigned char gray = 0;
    gray = 0.3 * (im->data)[i* im->cols +j].r + 0.59 * (im->data[i* im->cols +j]).g +
    0.11 * (im->data[i* im->cols +j]).b;
    if (gray < thresh) {
      im->data[i*im->cols + j] = black;
    }else {
      im->data[i*im->cols + j] = white;
    }
    }
  } 
  return 0;
}

/* crop
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * crops section of read image with user top left and bottom right coordinates
 * Return 0 if successful.
 */
int crop(Image *im, int top_col, int top_row, int bot_col, int bot_row){
  if ((bot_col > im->cols) || (bot_col < 0) || (bot_row > im->rows) || (bot_row < 0) || (top_col > im->cols) || (top_col < 0) || (top_row > im->rows) || (top_row < 0)){
          return 7;
  }
  
  if ((bot_col < top_col)||(bot_row < top_row)){
            return 7;
  }

  int crop_col_length = bot_col - top_col;
  int crop_row_length = bot_row - top_row;
  
  
  Pixel *crop = (Pixel*)malloc((int)sizeof(Pixel)*crop_col_length*crop_row_length);
  if (crop == NULL) {
        // Failed to allocate memory
        return 8;
  }

  // Perform the cropping operation
  int crpIndex = 0;
  for (int i = top_row; i < bot_row; i++) {
      for (int j = top_col; j < bot_col; j++) {
          int imIndex = i * im->cols + j;
          crop[crpIndex] = im->data[imIndex];
          crpIndex++;
      }
  }

  // Free the memory of the original image data
  free(im->data);

  // Update the image properties with the cropped dimensions and data
  im->cols = crop_col_length;
  im->rows = crop_row_length;
  im->data = crop;
  
  return 0;
}

/* transpose
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * converts grayscale image to black and white image
 * Return 0 if successful.
 */
int transpose(Image *im){
  //define new dimensions
  int new_row = im->cols;// new length
  int new_col = im->rows;// new width
  Pixel *transpose = malloc(sizeof(Pixel) * new_col * new_row);
  if (transpose == NULL) {
        // Failed to allocate memory
        return 8;
  }
  int transpose_index = 0;
  for (int i = 0; i < new_row;i++){
    for (int j = 0; j < new_col;j++){
      int old_index = j * new_row + i;//iterating image by referencing cols 
      transpose[transpose_index] = im->data[old_index];
      transpose_index++; 
    }
  }

  //free old data
  free(im->data);

  //set new dimensions
  im->rows = new_row; 
  im->cols = new_col;
  im->data = transpose;

  return 0;
}

/* gradient
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * converts grayscale image to gradient image
 * Return 0 if successful
 */
int gradient(Image *img) {
  grayscale(img); // convert image to greyscale

  // initialize variables for clarity
  int rows = img->rows;
  int cols = img->cols;
  unsigned char gradient_vals[rows*cols]; // used to store gradient values for each pixel; parallel to data array in img structure
  float gradx = 0;
  float grady = 0;
  
  // compute gradient values
  for (int i = 0; i < cols; i++) { // first row set to 0
    gradient_vals[i] = 0;
  }
  for (int i = ((rows - 1)*cols); i < (cols*rows); i++) { // last row set to 0
    gradient_vals[i] = 0;
  }
  for (int i = cols; i < ((rows - 1)*cols); i++) {
    if ((i % cols == 0) || ((i+1) % cols == 0)) { // set first and last col to 0
      gradient_vals[i] = 0;
    }
    else {
      gradx = (img->data[i+1].r - img->data[i-1].r) / 2;
      grady = (img->data[i-cols].r - img->data[i+cols].r) / 2;
      gradient_vals[i] = (unsigned char)(fabs(gradx) + fabs(grady));
      
    }
  }

  // set each color of each pixel to gradient value
  for (int i = 0; i < rows*cols; i++) {
    img->data[i].r = gradient_vals[i];
    img->data[i].b = gradient_vals[i];
    img->data[i].g = gradient_vals[i];
  }

  return (0);
}

/* helper function for seam carving
 * Read a PPM-formatted image from a file (assumes fp != NULL).
 * gets gradient values for seamcarving
 * Return 0 if successful
 */
int getGradientVals(Image *img, int return_ra[]) {
  // intialize variables for clarity
  int rows = img->rows;
  int cols = img->cols;

  // get grayscale values
  int gray_vals[rows*cols];
  for (int i = 0; i < rows*cols; i++) {
    gray_vals[i] = (int) ((0.3 * (img->data)[i].r) + (0.59 * (img->data)[i].g) + (0.11 * (img->data)[i].b)); 
  }

  float gradx = 0;
  float grady = 0;
  // calculate gradient values and populate return array
  for (int i = 0; i < cols; i++) { // first row set to 0
    return_ra[i] = 0;
  }
  for (int i = ((rows - 1)*cols); i < (cols*rows); i++) { // last row set to 0
    return_ra[i] = 0;
  }
  for (int i = cols; i < ((rows - 1)*cols); i++) {
    if ((i % cols == 0) || ((i+1) % cols == 0)) { // set first and last col to 0
      return_ra[i] = 0;
    }
    else {
      gradx = (gray_vals[i+1] - gray_vals[i-1]) / 2;
      grady = (gray_vals[i-cols] - gray_vals[i+cols]) / 2;
      return_ra[i] = (int)(fabs(gradx) + fabs(grady));   
    }
  }
  return (0);
}
    

/* removeSeam
 * Removes 1 column seam from an image (assume Image* != NULL)
 */
int removeSeam(Image *img) {
  // intialize variables for clarity
  int rows = img->rows;
  int cols = img->cols;

  //--------populate 2D seam array----------//
  // initialize necessary variables and arrays
  int grad_vals[rows*cols];
  getGradientVals(img, grad_vals); // populate array of gradient magnitudes
  int seams[cols-2][rows]; // 2D array to hold the seams; structured by storing indices of each pixel in an array, each array represting a seam
  int col_pos = 0; // variable to store what our column position is
  int min_grad = 0; // variable to find minimum gradient value
  
  for (int i = 0; i < cols - 1; i++) { // loop through the first column to the second to last column
    col_pos = i+1; // column position starts at each column index
    seams[i][0] = col_pos; // first index in each seam is the indice of the 0th row pixel of that column

    for (int j = 1; j < rows - 1; j++) { // loop from the 1st row to the second to last row
      if (col_pos == 1) { // if we are at the left border
	if (grad_vals[j*cols + col_pos + 1] < grad_vals[j*cols + col_pos]) {
	  col_pos++; // if right neighbor is lesser, move col_pos to right neighbor
	}
      }
      else if (col_pos == cols - 2) { // if we are at the right border
	if (grad_vals[j*cols + col_pos - 1] <= grad_vals[j*cols + col_pos]) {
	  col_pos--; // if left neighbor is lesser or equal, move col_pos to left neighbor
	}
      }
      else {
	min_grad = returnMin(grad_vals[j*cols + col_pos], grad_vals[j*cols + col_pos + 1], grad_vals[j*cols + col_pos - 1]);
	if (grad_vals[j*cols + col_pos - 1] == min_grad) {
	  col_pos--; // if left is the min, move col_pos left
	}
	else if (grad_vals[j*cols + col_pos] == min_grad) {
	  col_pos = col_pos; // if middle is min, do nothing
	}
	else if (grad_vals[j*cols + col_pos + 1] == min_grad) {
	  col_pos++; // if right is the min, move col_pos right
	}
      }
   
      seams[i-1][j] = j*cols + col_pos; // add the position of the min gradient to the seams array
    }

    seams[i-1][rows-1] = (rows-1)*cols + col_pos;
  }

  //---------------Find lowest energy seam----------------//
  // initialize variables to find the index of lowest energy seam
  int current_energy = 0;
  int smallest_energy = 0;
  int smallest_index = 0;
  for (int i = 0; i < rows; i++) { // set smallest seam energy to first seam by default
    smallest_energy += grad_vals[seams[0][i]];
  }

  for (int i = 1; i < cols-2; i++) { // loop through all seams to find the lowest energy index
    current_energy = 0;
    for (int j = 0; j < rows; j++) {
      current_energy += grad_vals[seams[i][j]]; // set current energy to sum of all magnitudes in the seam
    }
  
    if (current_energy < smallest_energy) { // if new smallest is found
      smallest_energy = current_energy; // update smallest energy
      smallest_index = i; // record index
    }
  }

  //--------------Edit image by removing seam-------------//
  // Initialize variables to edit the image
  Pixel *edited = (Pixel*)malloc(sizeof(Pixel) * (cols-1) * rows); // create new "data" array for Image structure with 1 less column
  if (edited == NULL) {
        // Failed to allocate memory
        return 8;
  }
  int edit_index = 0;
  int must_remove = 0; // keeps track of whether a given pixel needs to be removed while looping through

  for (int i = 0; i < rows*cols; i++) {
    must_remove = 0;
    for (int j = 0; j < rows; j++) { // loop through the seam to see if this is a pixel that should be removed
      if (i == seams[smallest_index][j]) {
	must_remove = 1; // if it is, make mustRemove true
      }
    }
    if (!must_remove) { // copy img data to edited data UNLESS it is a pixel that should be removed
      edited[edit_index] = img->data[i];
      edit_index++;
    }
  }

  
  
  // reasign image data to edited
  free(img->data);
  (img->cols) -= 1;
  (img->data) = edited;


  


  //------------Return 0-----------------------//
  
  return (0);
}


int seamCarve(Image *img, float col_size, float row_size) {
  
  if (col_size > 1 || col_size < 0 || row_size > 1 || row_size < 0){
    return 7;
  }
  // determine rows and cols to be removed
  int new_col = img->cols * col_size;
  int new_row = img->rows * row_size;
  int remove_col = img->cols - new_col;
  int remove_row = img->rows - new_row;
  // remove remove_col column seams
  for (int i = 0; i < remove_col; i++) {
    removeSeam(img);
  }

  // tranpose to remove rows, remove remove_row row seams
  transpose(img);
  for (int i = 0; i < remove_row; i++) {
    removeSeam(img);
  }
  transpose(img);

  return (0);
}



int returnMin(int a, int b, int c) {
  int min = a;
  if (b < min) {
    min = b;
  }
  if (c < min) {
    min = c;
  }
  return (min);
}

void printError(const int return_code, char *argv[]) {
  char *filename = argv[1];

  switch (return_code) {
    case 1:
      printf("Wrong usage(ie mandatory arguments are not provided\n");
      break;
    case 2:
      printf("Input file I/O error\n");
      break;
    case 3:
      printf("Output file I/O error\n");
      break;
    case 4:
      printf("The Input file cannot be read as a PPM file\n");
      break;
    case 5:
      printf("Unsupported image processing operations\n");
      break;
    case 6:
      printf("Incorrect number of arguments for the specified operation\n");
      break;
    case 7:
      printf("Invalid arguments for the specified operation\n");
      break;
    default:
      printf("Other errors not specified above in %s\n",filename);
  }
}
			    

