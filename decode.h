/**
 * @file: decode.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Selmes  @student_id: 12545253
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
#ifndef ENCODE_H
#define ENCODE_H

 /**
 * @name getImageList
 * @brief Should take a directory and get a list of images to be processed. 
 *
 * @developer:
 *
 */
void getImageList(void);

 /**
 * @name getImageList
 * @brief Should take an image name, read the file in, and decode it into some 
 * kind of pixel array.  
 *
 * @developer:
 *
 */
void decodeImage(const char imgFileName[], char **rgbaArray_p, 
	int *width_p, int *height_p);

#endif
