/**
 * @file: resize.h
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Bui     @student_id: 12518371
 *   @name: Daniel Selmes  @student_id: 12545253
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @achieved:
 *
 * @pending:
 *
 * @note:
 *
 * @TODO:
 *
 */


/* Include Guard. */
#ifndef RESIZE_H
#define RESIZE_H

/**
 * @name ratioCalculator
 * @brief accepts the current image width and height and returns a new height
 * proportional to the new width. 
 *
 * @developer:
 *
 * @param width
 * @param height
 * @param targetWidth
 * @param newWidth
 * @param newHeight
 * 
 */
float ratioCalculator(int width, int height, int targetWidth, int *newWidth_p, int *newHeight_p);

/**
 * @name imageResizer
 * @brief accepts an rgba pixel buffer and returns a resized one
 *
 * @developer:
 *
 * @param rgbaBufferIn
 * @param rgbaBufferOut
 * @param width
 * @param height
 */
void imageResizer(const char *rgbaBufferIn, char **rgbaBufferOut, int width, int height);


#endif
