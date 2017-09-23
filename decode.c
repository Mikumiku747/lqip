/**
 * @file: decode.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Selmes  @student_id: 12545253
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 18-09-2017
 *
 * @TODO:
 *
 */
 
/* Include the header for this module. */
#include "decode.h"
/* Includes for other system headers. */
#include <stdio.h>
/* fopen, fprintf, stderr */
#include <stdlib.h>
/* malloc */
 
/* Function declarations for internal functions. */
void bmpDecode(FILE *imgFile, char **rgbaArray_p, int *width_p, int *height_p);
 
/**
 * @name getImageList
 * @brief Should take a directory and get a list of images to be processed. 
 *
 * See header for details.
 */
void getImageList(void);

/**
 * @name getImageList
 * @brief Should take an image name, read the file in, and decode it into some 
 * kind of pixel array.  
 *
 * See header for details.
 */
void decodeImage(
	const char imgFileName[], 
	char **rgbaArray_p, 
	int *width_p, 
	int *height_p
	) {
	/* File pointer to keep track of the file name. */
	FILE *imgFile; 
	/* Char buffer pointer which we'll fill in. */
	char *imgData_p;
	
	/* Set defaults for the results so we can return if we encounter an error. */
	*rgbaArray_p = NULL;
	*width_p = NULL;
	*height_p = NULL;
	
	/* Open the image file to determine the type of file it is. */
	imgFile = fopen(imgFileName, "r");
	if (!imgFile) {
		fprintf(stderr, "Error reading image file %s\n", imgFileName);
		return;
	}
	/* Read in the first 4 bytes. */
	imgData_p = malloc(sizeof(char)*4);
	if (!imgData_p) {
		fprintf(stderr, "Error allocating memory\n");
		fclose(imgFile);
		return;
	}
	if (fread(imgData_p, sizeof(char), 4, imgFile) != 4) {
		fprintf(stderr, "Error reading image file\n");
		free(imgData_p);
		fclose(imgFile);
		return;
	}
	/* Now, check those first 4 bytes to see if we can ID the file type. */
	if (imgData_p[0] == 'B' && imgData_p[1] == 'M') {
		/* This is likely a Bitmap file, let's hand it to the BMP decoder to 
		check. */
		free(imgData_p);
		/* Move back to the start of the file. */
		fseek(imgFile, 0, SEEK_SET);
		printf("bmp decoder used on %s\n", imgFileName);
		/*bmpDecode(imgFile, rgbaArray_p, width_p, height_p);*/
	}
}

/**
 * @name bmpDecode
 * @brief Decodes a BMP file format image into an rbga array
 */
void bmpDecode(FILE *imgFile, char **rgbaArray_p, int *width_p, int *height_p) {
	
}
