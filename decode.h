/**
 * @file: decode.c
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
 
 /*
 * decode.h
 * Developer: Daniel Selmes
 * Student #: 12545253
 * Generates a list of files to be decoded and decodes them from thier native
 * file format into a pixel array. 
 * 
 * Main tasks:
 * 
 * 	Image list handler: Should take a directory and get a list of images to 
 * 		be processed. 
 * 	
 * 	Image decoder: Should take an image name, read the file in, and decode it
 * 		into some kind of pixel array. 
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
void decodeImage(void);

#endif
