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
#ifndef ENCRYPTION_H
#define ENCRYPTION_H


/**
 * @name encryptFile
 * @brief encrypts file
 *
 * @developer:
 *
 */
char *encryptFile(char plaintext[], int textLen, char password[]);

/**
 * @name decryptsFile
 * @brief decrypts file
 *
 * @developer:
 *
 */
char *decryptFile(char plaintext[], int textLen, char password[]);


#endif
