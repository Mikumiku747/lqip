/**
 * @file: main.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Zouhir Chahoud @student_id: 11763745
 *   @name: Daniel Selmes  @student_id: 12545253
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
/* printf, fprintf, sprintf, stderr, fopen, fread, fwrite, cflose, fseek */
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "resize.h"
#include "argparse.h"
#include "base64.h"
#include "encrypt.h"
#include "encode.h"

/* Macro to avoid freeing NULL pointers. Whenever this is used, it is because
a pointer may or may not actually be allocated to begin with, and freeing an
invalid pointer will crash the program. */
#define SAFEFREE(ptr) if (ptr) {free(ptr);}

/* Quick control macros to disable excess print statements. */
/*#define VERBOSE_OUTPUT*/
/*#define DEBUG_OUTPUT*/

/* Forward declarations of functions defined in main.c. These too small or
too general to need thier own library. */
int testext(char *filename, char *ext);


/* Main program thread of execution. */
int main(int argc, char **argv) {
	
	/* First, check if the minimum args are specified, and if not, exit. */
	char *imgName = checkOption('i', argc, argv);
	char *dirName = checkOption('d', argc, argv);
	char *targetSize = checkOption('s', argc, argv);
	
	/* Check for the decryption mode. */
	char *decryptionKey = checkOption('u', argc, argv);
	if (decryptionKey) {
		/* Attempt to perform decryption. First, read the encrypted file. */
		FILE *cryptFile = fopen("placeholders.enc", "r");
		if (!cryptFile) {
			fprintf(stderr, "Error opening encrypted placeholders "
			"(check \"placeholders.enc\" exists and is readable.\n");
			SAFEFREE(decryptionKey);
			return 1;
		}
		/* Now attempt to open the file to write it to. */
		FILE *unencOutFile = fopen("placeholders.yaml", "w");
		if (!unencOutFile) {
			fprintf(stderr, "Error opening new unencrypted placeholders.yaml"
			" file (check write permissions).\n");
			SAFEFREE(decryptionKey);
			fclose(cryptFile);
			return 1;
		}
		/* Get the length of the encrypted file and read the whole thing. */
		fseek(cryptFile, 0, SEEK_END);
		size_t encSize = ftell(cryptFile);
		fseek(cryptFile, 0, SEEK_SET);
		char *encBuffer = malloc(sizeof(char)*encSize);
		if (!encBuffer) {
			fprintf(stderr, "Error allocating memory for encryption buffer.\n");
			SAFEFREE(decryptionKey);
			fclose(unencOutFile);
			fclose(cryptFile);
			return 1;
		}
		size_t dataRead = fread(encBuffer, sizeof(char), encSize, cryptFile);
		if (dataRead != encSize) {
			fprintf(stderr, "Error reading in encrypted file.\n");
			SAFEFREE(decryptionKey);
			SAFEFREE(encBuffer);
			fclose(unencOutFile);
			fclose(cryptFile);
			return 1;
		}
		/* Decrpyt the whole thing. */
		char *decryptText = decryptFile(encBuffer, encSize, decryptionKey);
		if (!decryptText) {
			fprintf(stderr, "Error in decryption.\n");
			SAFEFREE(decryptionKey);
			SAFEFREE(encBuffer);
			fclose(unencOutFile);
			fclose(cryptFile);
			return 1;
		}
		if (memcmp(decryptText, "<yaml>", sizeof(char)*6) != 0) {
			fprintf(stderr, "Warning: Decryption mismatch detected, decryption "
			"likely failed due to an incorrect password.\n");
		}
		/* Write it to the output. */
		size_t dataWrote = fwrite(decryptText, sizeof(char), encSize, unencOutFile);
		if (dataWrote != encSize) {
			fprintf(stderr, "Error writing plaintext to placeholders.enc "
			"(check write permissions).\n");
			SAFEFREE(decryptionKey);
			SAFEFREE(encBuffer);
			fclose(unencOutFile);
			fclose(cryptFile);
			return 1;
		}
		/* Free up all the resources. */
		SAFEFREE(decryptionKey);
		SAFEFREE(encBuffer);
		SAFEFREE(decryptText);
		fclose(unencOutFile);
		fclose(cryptFile);
		return 1;
	}
	if (!targetSize) {
		/* Give a hint about usage (they forgot the size option). */
		fprintf(stderr, "Error: No size provided (use -s option)\n"
		"Usage: \tlqip (-i imagename | -d directory) -s size [-e passkey]\n"
			   "\tlqip -u passkey\n");
		SAFEFREE(imgName);
		SAFEFREE(dirName);
		SAFEFREE(targetSize);
		return 1;
	}
	if ((!imgName && !dirName) || (imgName && dirName)) {
		/* Give a hint about usage (they forgot the input files). */
		fprintf(stderr, "Error: Please specify one of -i or -d options\n"
		"Usage: lqip (-i imagename | -d directory) -s size [-e passkey]\n"
			   "\tlqip -u passkey\n");
		SAFEFREE(imgName);
		SAFEFREE(dirName);
		SAFEFREE(targetSize);
		return 1;
	}
	
	/* Next, convert the target size to an integer, and attempt to load / 
	resize the image. Check it converted right. */
	int target = atoi(targetSize);
	if (target < 1) {
		fprintf(stderr, "Invalid size: %s\n", targetSize);
		SAFEFREE(imgName);
		SAFEFREE(dirName);
		SAFEFREE(targetSize);
		return 1;
	}
	/* Free the string now that we have an int to work with. */
	free(targetSize);
	
	/* If specified, do a single image conversion */
	if (imgName) {
#ifdef DEBUG_OUTPUT
		/*DEBUG*/printf("Converting %s to a thumbnail of width %d\n", imgName, target);
#endif
		
		/* Storage for image dimensions. */
		int widthBig, heightBig, widthSml, heightSml;
		/* Buffers for the big and small copies of the images. */
		char *rgbaBufferBig;
		char *rgbaBufferSml;
		char *bmpBuffer;
		
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
		
		/* Encode the image into a BMP file. */
		bmpBuffer = encodeImageBmp(rgbaBufferSml, widthSml, heightSml);
		if (!bmpBuffer) {
			fprintf(stderr, "BMP encoder error, exiting.\n");
			free(imgName);
			free(rgbaBufferBig);
			free(rgbaBufferSml);
			return 1;
		}
#ifdef DEBUG_OUTPUT
		/* Attempt to output the image data to a file to test it worked. */
		char *newName = malloc(sizeof(char)*strlen("%s-sml.bmp")+
			sizeof(char)*strlen(imgName));
		sprintf(newName, "%s-sml.bmp", imgName);
		FILE *testImgFile = fopen(newName, "w");
		fwrite(bmpBuffer, sizeof(char), 54 + widthSml*heightSml*4, testImgFile);
		fclose(testImgFile);
#endif
		
		/* Encode the result into a base64 string. I allocate 1.4 times the	number
		of bytes in the RGBA array since the inflation ratio when encoding as 
		base64 is 4 characters per 3 btyes, or 1.33, so 1.4 is on the safe side. 
		*/
		int inputBufferSize = widthSml * heightSml * 4 + 54;
		char b64String[(int)(inputBufferSize * 1.4)];
		encode(bmpBuffer, inputBufferSize, b64String);
		
		/* Display the resulting YAML to the command line: */
#ifdef VERBOSE_OUTPUT
			printf("\nOutput Data\n\n<yaml>\n\t<img>"
			"\n\t\t<name>%s</name>"
			"\n\t\t<data>%s</data>"
			"\n\t\t<format>rgba</data>"
			"\n\t</img>\n</yaml>\n", 
			imgName, b64String);
#endif

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
	}
	
	/* If specified, do batch image conversion. */
	if (dirName) {
		/* Scan a directory for image files and put them in a linked list. */
		imgData_t *listHead = getImageList(dirName);
		if (!listHead) {
			fprintf(stderr, "Error reading directory: %s\n", dirName);
			free(dirName);
			SAFEFREE(imgName);
			return 1;
		}
		/* Now that we have the files, let's try to process them. */
		imgData_t *curNode;
		for (curNode = listHead; curNode != NULL; curNode = curNode->next) {
			/* Decode the image. */
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Decoding %s\n", curNode->fileName);
#endif
			decodeImage(curNode->fileName, &(curNode->rgbaBufferBig), 
			&(curNode->widthBig), &(curNode->heightBig));
			if (!(curNode->rgbaBufferBig)) {
				fprintf(stderr, "Error decoding %s, no thumbnail generated.\n", curNode->fileName);
				curNode->failedProcessing = 1;
				continue;
			}
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Done.\n");
#endif
			/* Resize the image. */
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Resizing %s\n", curNode->fileName);
#endif
			imageResizer(curNode->rgbaBufferBig, curNode->widthBig, curNode->heightBig, 
				&(curNode->rgbaBufferSml), &(curNode->widthSml), &(curNode->heightSml), target);
			if (!(curNode->rgbaBufferSml)) {
				fprintf(stderr, "Error resizing %s, no thumbnail generated.\n", curNode->fileName);
				curNode->failedProcessing = 1;
				continue;
			}
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Done.\n");
			
			/* Do the bmp encoding. */
			curNode->bmpBuffer = encodeImageBmp(curNode->rgbaBufferSml, 
				curNode->widthSml, curNode->heightSml);
#ifdef DEBUG_OUTPUT
			/* Attempt to output the image data to a file to test it worked. */
			char *newName = malloc(sizeof(char)*strlen("%s-sml.bmp")+
				sizeof(char)*strlen(curNode->fileName));
			sprintf(newName, "%s-sml.bmp", curNode->fileName);
			FILE *testImgFile = fopen(newName, "w");
			fwrite(curNode->bmpBuffer, sizeof(char), 54 + curNode->widthSml * curNode->heightSml*4, testImgFile);
			fclose(testImgFile);
#endif
			/* Encoding to base64. */
			/*DEBUG*/printf("Encoding %s base64...", curNode->fileName);
#endif
			/* Allocate the output buffer for the base64 bytes. */
			curNode->base64Buffer = malloc(sizeof(char) * (4 * curNode->widthSml * curNode->heightSml + 54) * 1.4);
			if (!curNode->base64Buffer) {
				fprintf(stderr, "Error allocating base64 buffer for %s\n", curNode->fileName);
				curNode->failedProcessing = 1;
				continue;
			}
			encode(curNode->bmpBuffer, curNode->widthSml*curNode->heightSml*sizeof(char)*4 + 54, curNode->base64Buffer);
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Done.\n");
#endif
		}
		/* Now that we've processed all the images, report passes/fails. */
#ifdef VERBOSE_OUTPUT
		printf("\nThe following files processed successfully:\n");
		for (curNode = listHead; curNode != NULL; curNode = curNode->next) {
			if (!curNode->failedProcessing) {
				printf("%s\n", curNode->fileName);
			}
		}
		printf("\nThe following files failed to process:\n");
		for (curNode = listHead; curNode != NULL; curNode = curNode->next) {
			if (curNode->failedProcessing) {
				printf("%s\n", curNode->fileName);
			}
		}
#endif
		/* Begin outputting the yaml file. */
		FILE *outfile = fopen("placeholders.yaml", "w");
		if (!outfile) {
			fprintf(stderr, "Error opening output file placeholders.yaml\n");
			return 1;
		}
#ifdef VERBOSE_OUTPUT
		printf("\nOutput Data\n\n<yaml>\n");
#endif
		fprintf(outfile, "<yaml>\n");
		/* Output each image's data. */
		for (curNode = listHead; curNode != NULL; curNode = curNode->next) {
			if (!curNode->failedProcessing) {
#ifdef VERBOSE_OUTPUT
				printf("\t<img>\n\t\t<name>%s</name>\n\t\t<data>%s</data>\n"
					"\t\t<format>rgba</format>\n\t</img>\n",
					curNode->fileName, curNode->base64Buffer);
#endif
				fprintf(outfile, "\t<img>\n\t\t<name>%s</name>\n\t\t<data>%s</data>\n"
					"\t\t<format>rgba</format>\n\t</img>\n",
					curNode->fileName, curNode->base64Buffer);
			}
		}
		/* Put an end on the YAML. */
#ifdef VERBOSE_OUTPUT
		printf("</yaml>\n");
#endif
		fprintf(outfile, "</yaml>\n");
		fclose(outfile);
		
		/* And finally, we need to go in and free up all the resources we've 
		been taking up!... */
		for (curNode = listHead; curNode != NULL; curNode = curNode->next) {
			/* First off, we just free up the buffers. */
			SAFEFREE(curNode->fileName);
			SAFEFREE(curNode->rgbaBufferBig);
			SAFEFREE(curNode->rgbaBufferSml);
			SAFEFREE(curNode->base64Buffer);
		}
		/* Now, this time, we actually free up the linked list. */
		imgData_t *nextNode_p = listHead;
		curNode = NULL;
		while (nextNode_p != NULL) {
			curNode = nextNode_p;
			nextNode_p = curNode->next;
			free(curNode);
		}
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
	}
	return 0;
}


