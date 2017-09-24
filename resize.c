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
	/*DEBUG*/printf("Ratio: %f\n", ratio);
	if (newWidth_p) {
		*newWidth_p = (int)((float)width / ratio);
	}
	if (newHeight_p) {
		*newHeight_p = (int)((float)height / ratio);
		/*DEBUG*/printf("Old: %dx%d New: %dx%d\n", width, height, *newWidth_p, *newHeight_p);
	}	
	return ratio;
}

/**
 * @name imageResizer
 * @brief accepts an rgba pixel buffer and returns a resized one
 *
 * See Header for details. 
 */
void imageResizer(const char *rgbaBufferIn, char **rgbaBufferOut, int width, int height);
