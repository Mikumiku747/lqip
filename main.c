/**
 * @file: main.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "resize.h"
#include "argparse.h"
#include "base64.h"
#include "encrypt.h"

/* Macro to avoid freeing NULL pointers. */
#define SAFEFREE(ptr) if (ptr) {free(ptr);}

int main(int argc, char **argv) {
	
	/* First, check if the minimum args are specified, and if not, exit. */
	char *imgName = checkOption('i', argc, argv);
	char *targetSize = checkOption('s', argc, argv);
	if (!imgName || !targetSize) {
		/* Give a hint about usage. */
		fprintf(stderr, "Usage: lqip -i image.bmp -s size [-e passkey]\n");
		SAFEFREE(imgName);
		SAFEFREE(targetSize);
		return 1;
	}
	
	/* Next, convert the target size to an integer, and attempt to load / 
	resize the image. Check it converted right. */
	int target = atoi(targetSize);
	if (target < 1) {
		fprintf(stderr, "Invalid size: %s\n", targetSize);
		free(imgName);
		free(targetSize);
		return 1;
	}
	/* Free the string now that we have an int to work with. */
	free(targetSize);
	/*DEBUG*/printf("Converting %s to a thumbnail of width %d\n", imgName, target);
	/* Storage for image dimensions. */
	int widthBig, heightBig, widthSml, heightSml;
	/* Buffers for the big and small copies of the images. */
	char *rgbaBufferBig;
	char *rgbaBufferSml;
	/* Decode the image, check it really happened. */
	decodeImage(imgName, &rgbaBufferBig, &widthBig, &heightBig);
	if (!rgbaBufferBig) {
		fprintf(stderr, "Decode error, exiting.\n");
		free(imgName);
		return 1;
	}
	/* Resize it. */
	imageResizer(rgbaBufferBig, widthBig, heightBig, &rgbaBufferSml, &widthSml, &heightSml, target);
	if (!rgbaBufferSml) {
		fprintf(stderr, "Resize error, exiting.\n");
		free(imgName);
		free(rgbaBufferBig);
		return 1;
	}
	/* Free the original image now that we have the thumbnail. */
	/* Encode the result into a base64 string. I allocate 1.4 times the	number
	of bytes in the RGBA array since the inflation ratio when encoding as 
	base64 is 4 characters per 3 btyes, or 1.33, so 1.4 is on the safe side. 
	*/
	char b64String[strlen(rgbaBufferSml) + 1];
	encode(rgbaBufferSml, b64String);
	/* Display the resulting YAML to the command line: */
	printf("\n\n<yaml>\n\t<img>"
		"\n\t\t<name>%s</name>"
		"\n\t\t<data>%s</data>"
		"\n\t\t<format>rgba</data>"
		"\n\t</img>\n</yaml>\n", 
		imgName, b64String);

	/* Save them to a file: */
	FILE *outfile = fopen("placeholders.yaml", "w");
	fprintf(outfile, "\n\n<yaml>\n\t<img>"
		"\n\t\t<name>%s</name>"
		"\n\t\t<data>%s</data>"
		"\n\t\t<format>rgba</data>"
		"\n\t</img>\n</yaml>\n",
		imgName, b64String);
	fclose(outfile);

	/* If encryption was asked for, use it. */	
	char *enkey = checkOption('e', argc, argv);
	if (enkey) {
		printf("Encrypted output saved to placeholders.enc\n");
		/* In order to actually accomplish the encryption, we will read the 
		plaintext output and then encrypt that. */
		/* Read in the file. */
		FILE *encfile = fopen("placeholders.yaml", "r");
		fseek(encfile, 0, SEEK_END);
		int size = ftell(encfile);
		fseek(encfile, 0, SEEK_SET);
		char *ptext = malloc(sizeof(char)*size);
		fread(ptext, sizeof(char), size, encfile);
		fclose(encfile);
		/* Encyrypt. */
		char *etext = encryptFile(ptext, size, enkey);
		/* Write. */
		encfile = fopen("placeholders.enc", "w");
		fwrite(etext, sizeof(char), size, encfile);
		fclose(encfile);
		free(ptext);
		free(etext);
		free(enkey);
	}

	/* Release any resources we're still holding. */
	/**
	 * we are not allocating this
	 * free(b64String);
	**/ 

	return 0;
	
}
