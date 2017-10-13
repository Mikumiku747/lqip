/**
 * @file: encode.h
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

char bmpHeaderData[54];

/**
 * @name encodeImageBmp
 * @brief Takes an RGBA array and encodes it as a BMP image file 
 *
 * @developer
 * @param rgbaArray
 * @param width
 * @param height
 * 
 */
char *encodeImageBmp(const char *rgbaArray, int width, int height);

#endif