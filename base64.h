/**
 * @file: base64.h
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
 * @TODO: Check return types from other developers
 *
 */

#include <stdio.h>
#include <string.h>
 
/* Include Guard. */
#ifndef BASE64_H
#define BASE64_H

/**
 * @name encode
 *
 * @developer Zouhir Chahoud 11763745
 *
 * @param input_str input string from image buffer
 * @param b64_output base64 encoded output string
 *
 * @description: this function encodes a string to Base64 and adds padding if any was needed
 *
 * @TODO: research if unsigned char is the best return value for consumers
 */
void encode(char *input_str, char *b64_output);


/**
 * @name encode chunk
 *
 * @developer Zouhir Chahoud 11763745
 *
 * @param in_chunk string input
 * @param b64_out_chunk base64 converted output
 * @param len size
 *
 * @description: will encode every 3 8-Bit binary bytes into 4 6-bit character
 *
 * @TODO: research if unsigned char is the best return value for consumers
 *
 */
void encodeChunk( unsigned char in_chunk[], char b64_out_chunk[], int len );


/**
 * @name decode
 *
 * @developer Zouhir Chahoud 11763745
 *
 * @param b64_input
 * @param output_str
 *
 * @description: this function encodes a string to Base64 and adds padding if any was needed
 *
 * @TODO: research if unsigned char is the best return value for consumers
 */
void decode(char *b64_input, char *output_str);


/**
 * @name decode chunk
 *
 * @developer Zouhir Chahoud 11763745
 *
 * @param in_chunk string input
 * @param b64_out_chunk base64 converted output
 * @param len size
 *
 * @description: will encode every 4 6-bit character to 3 8-Bit binary bytes
 *
 * @TODO: research if unsigned char is the best return value for consumers
 *
 */
void decodeChunk(unsigned char in_chunk[], char *output_chunk);

#endif