/**
 * @file: encrypt.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Owen Buckingham@student_id: 12466076
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @TODO:
 *
 */
 
#include "encrypt.h"

#include <stdlib.h>
/* malloc, free */
#include <string.h>
/* strlen */

/**
 * @name encryptFile
 * @brief encrypts file
 *
 * See Header for details
 *
 */
char *encryptFile(char plaintext[], int textLen, char password[]) {
	/* We use an XOR algorithm to obscure the plaintext. */
	/* Loop counters for the plaintext and password. */
	int ptext_count, pwcount;
	pwcount = 0;
	
	
	
	char *output = malloc(sizeof(char)*textLen);
	if (output == NULL) {
		return NULL;
	}
	
	for (ptext_count = 0; ptext_count < textLen; ptext_count++) {
		output[ptext_count] = plaintext[ptext_count] ^ password[pwcount];
		pwcount++;
		if ((size_t)pwcount >= strlen(password)) {
			pwcount = 0;
		}
	}
	
	return output;
}

/**
 * @name decryptsFile
 * @brief decrypts file
 *
 * See Header for details
 *
*/
char *decryptFile(char encryptText[], int textLen, char password[]) {
	return encryptFile(encryptText, textLen, password);
}
