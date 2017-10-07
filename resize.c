/**
 * @file: resize.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Bui     @student_id: 12518371
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @TODO:
 *
 */

/* Include the header for this module. */
#include "resize.h"

/* Include the other necessary system libaries. */
#include <stdlib.h>
/* NULL, malloc, free */
#include <stdio.h>
/* fprintf, sterr*/

/* Macro for controlling excess print output. */
/*#define DEBUG_OUTPUT*/

/**
 * @name ratioCalculator
 * @brief accepts the current image width and height and returns the ratio 
 * between the new and old size, depending on the target size. Also stashes 
 * the new dimensions in the newWidth and newHeight pointers if they're valid.
 *
 * See Header for details. 
 */
float ratioCalculator(int width, int height, int targetWidth, int *newWidth_p, int *newHeight_p) {
	/* We simply find the ratio between old and new width and apply that to 
	scale the old height to the new height. */
	float ratio = (float)width / (float)targetWidth;
#ifdef DEBUG_OUTPUT
	/*DEBUG*/printf("Ratio: %f\n", ratio);
#endif
	if (newWidth_p) {
		*newWidth_p = (int)((float)width / ratio);
	}
	if (newHeight_p) {
		*newHeight_p = (int)((float)height / ratio);
#ifdef DEBUG_OUTPUT
		/*DEBUG*/printf("Old: %dx%d New: %dx%d\n", width, height, *newWidth_p, *newHeight_p);
#endif
	}	
	return ratio;
}

/**
 * @name imageResizer
 * @brief accepts an rgba pixel buffer and returns a resized one
 *
 * See Header for details. 
 */
void imageResizer(const char *rgbaBufferIn, int widthIn, int heightIn, 
	char **rgbaBufferOut_p, int *widthOut_p, int *heightOut_p, 
	int targetWidth) {
	/* Use two loops to iterate through each of the images and access pixels
	at a ratio faster in one image than the other. */

	/* Loop counters. */
	int row, col;


	/* Calculate the ratio to move through the source compared to the 
	original. */
	float ratio = ratioCalculator(widthIn, heightIn, targetWidth, widthOut_p, heightOut_p);
	int ratioI = (int)ratio;

	/* Allocate space for the new resized buffer. */
	*rgbaBufferOut_p = malloc(sizeof(char) * 4 * (*widthOut_p)*(*heightOut_p));
	if (!*rgbaBufferOut_p) {
		fprintf(stderr, "Error allocating memory!\n");
		return;
	}
	
	/* Fill in each row of the destination. */
	for (row = 0; row < *heightOut_p; row++) {
		/* Fill in each pixel of the destination. */
		for (col = 0; col < *widthOut_p; col++) {
			/* We copy from a multiplied position in the source to the direct 
			position in the offset. */
			/* RED. */ (*rgbaBufferOut_p)[
				row * (*widthOut_p) * 4 +
				col * 4 + 0
			] = rgbaBufferIn[
				row * (widthIn) * 4 * ratioI +
				col * 4 * ratioI + 0
			];
			/* GREEN. */ (*rgbaBufferOut_p)[
				row * (*widthOut_p) * 4 +
					col * 4 + 1
			] = rgbaBufferIn[
				row * (widthIn) * 4 * ratioI +
					col * 4 * ratioI + 1
			];
			/* BLUE. */ (*rgbaBufferOut_p)[
				row * (*widthOut_p) * 4 +
					col * 4 + 2
			] = rgbaBufferIn[
				row * (widthIn) * 4 * ratioI +
					col * 4 * ratioI + 2
			];
			/* ALPHA. */ (*rgbaBufferOut_p)[
				row * (*widthOut_p) * 4 +
					col * 4 + 3
			] = rgbaBufferIn[
				row * (widthIn) * 4 * ratioI +
					col * 4 * ratioI + 3
			];
		}
	}
	/* all went well, we done. */

}
