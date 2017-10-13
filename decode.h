/**
 * @file: decode.h
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
#ifndef DECODE_H
#define DECODE_H

/* Linked list nodes to keep data associated with an image in one place. */
struct imgData {
	char *fileName;
	char *rgbaBufferBig;
	char *rgbaBufferSml;
	int widthBig;
	int heightBig;
	int widthSml;
	int heightSml;
	char *bmpBuffer;
	char *base64Buffer;
	int failedProcessing;
	struct imgData *next;
};
typedef struct imgData imgData_t;

 /**
 * @name getImageList
 * @brief Should take a directory and get a list of images to be processed. 
 *
 * @developer:
 *
 */
imgData_t *getImageList(char *dirName);

 /**
 * @name decodeImage
 * @brief Should take an image name, read the file in, and decode it into some 
 * kind of pixel array.  
 *
 * @developer:
 *
 */
void decodeImage(const char imgFileName[], char **rgbaArray_p, 
	int *width_p, int *height_p);

/**
	@name testext
	@brief Tests a filename to see if it ends in the given extension. 
	
	@param char *filename: the name of the file to testext
	@param char *ext: the file extension to test for
*/
int testext(char *filename, char *ext);

#endif
