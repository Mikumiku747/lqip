/**
 * @file: argparse.h
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
#ifndef ARGPARSE_H
#define ARGPARSE_H

/**
 * @name checkFlag
 * @brief Checks for a specific flag on the command line
 * 
 * @param flagChar
 * @param argc
 * @param argv
 *
 * @developer: Daniel Selmes
 *
 */
int checkFlag(char flagChar, int argc, char **argv);


/**
 * @name checkOption
 * @brief Checks for a specific flag on the command line
 * 
 * @param flagChar
 * @param argc
 * @param argv
 *
 * @developer: Daniel Selmes
 *
 */
char *checkOption(char flagChar, int argc, char **argv);

#endif

