/**
 * @file: resize.h
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Zouhir Chahoud @student_id: 11763745
 *   @name:                @student_id:
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
 * @brief accepts the current image width and height and returns new width and height values keeping the ratio
 *
 * @developer:
 *
 * @param width
 * @param height
 * @param newWidth
 * @param newHeight
 */
void ratioCalculator(int width, int height, int *newWidth, int *newHeight);

/**
 * @name imageResizer
 * @brief accepts image URI and returns a resized one
 *
 * @developer:
 *
 * @param uri
 * @param width
 * @param height
 */
void imageResizer(char *uri, int width, int height);


#endif
