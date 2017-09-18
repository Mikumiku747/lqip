/**
 * @file: argparse.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Selmes  @student_id: 12545253
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 18-09-2017
 *
 * @TODO:
 *
 */

/* Include the header for this code. */
#include "argparse.h"

/**
 * @name checkFlag
 * @brief Checks for a specific flag on the command line
 * 
 * See header for details.
 */
int checkFlag(char flagChar, int argc, char **argv) {
	/* Loop counter for command line argument. */
	int currentArg;
	
	/* argv is an array of strings, each one being one of the command line
	parameters. It has argc elements in it. The first element is the name
	of the calling program. A flag is a stirng that looks like "-c" where
	c is any lowercase or uppercase character or number. We search argv
	until we find a string that looks like the one we're searching for. */

	/* Loop through the command line options. */
	for (currentArg = 1; currentArg < argc; currentArg++) {
		/* Check if this is a flag (if it's a -) */
		if (argv[currentArg][0] == '-') {
			/* Now check if it's the flag we're looking for. */
			if (argv[currentArg][1] == flagChar) {
				return 1;
			}
		}
	}
}


/**
 * @name checkOption
 * @brief Checks for a specific flag on the command line
 * 
 * See Header for details.
 */
char *checkOption(char flagChar, int argc, char **argv) {
	
}
