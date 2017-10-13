/**
 * @file: decode.c
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
 
/* Include the header for this module. */
#include "decode.h"
/* Includes for other system headers. */
#include <stdio.h>
/* fopen, fread, flcose, printf, fprintf, sprintf, stderr */
#include <stdlib.h>
/* malloc, free, system */
#include <stdint.h>
/* Fixed width integer types (uint8_t, uint32_t, size_t, etc.) */
#include <string.h>
/* strlen, strcat, strcopy. */

/* Macro to control the amount of debug statements. */
/*#define DEBUG_OUTPUT*/

/* Explicit declaration of strdup since it's not always included in the
string header. */
char *strdup(const char *str);

/* Function declarations for internal functions. */
void bmpDecode(FILE *imgFile, char **rgbaArray_p, int *width_p, int *height_p);
int intPow(int n, int exponent);
 
/**
 * @name getImageList
 * @brief Should take a directory and get a list of images to be processed. 
 *
 * See header for details.
 */
imgData_t *getImageList(char *dirName) {
	/* Since there's an abitrary limit on what system libraries we can use,
	we can't use dirent.h, so we have to instead use this idiotic and hacky
	method of getting a list of files instead. If we had access to more of 
	the C standard library, this code would be much cleaner and efficient.
	
	Technically, you can just provide your own headers for system libraries
	and it doesn't break the rules of the assignment, but that's probably
	against the spirit of the rules, and so I'm going to avoid it here and
	just use a garbage hack instead. 
	
	Anyway, the actual method here is to outsource the call to the ls program,
	and then read the file names back in from the output file. */
	
	/* Pointers to the linked list nodes. */
	imgData_t *listHead = NULL;
	imgData_t *prevNode = NULL;
	imgData_t *curNode = NULL;
	const char commandFormat[] = "ls %s | grep .bmp |\
								 tee listing.txt | grep forcenomatch";
	
	/* Construct the command line string. */
	char *command = malloc(
		(strlen(commandFormat) + 
		strlen(dirName))*sizeof(char)
	);
	if (!command) {
		fprintf(stderr, "Error allocating memory for ls command.\n");
		return NULL;
	}
	/* Use sprintf to construct the command. */
	sprintf(command, commandFormat, dirName);
#ifdef DEBUG_OUTPUT
	printf("Running command \"%s\" to get file list.\n", command);
#endif
	system(command);
	/* Open the file list for reading. */
	FILE *fileList = fopen("listing.txt", "r");
	if (!fileList) {
		fprintf(stderr, "Error reading file listing\n");
		free(command);
		return NULL;
	}
	/* Buffer to hold the current line. 4096 should suffice for a max line 
	length. */
	char lineBuffer[4096];
	/* Read entries from the directory. */
	while (fscanf(fileList, "%[^\n]", lineBuffer) > 0) {
		getc(fileList); /* Advance the file past the newline. */
		/* First, ignore if it isn't an image file (bmp or gif extension). */
		if (lineBuffer[0] == '.' ||
			(!testext(lineBuffer, ".bmp") &&
			 !testext(lineBuffer, ".gif"))
		) {
			continue; 
		}
		/* We should at least try to process this file. Allocate a node
		for it. */
#ifdef DEBUG_OUTPUT
		/*DEBUG*/printf("Tracking %s...", lineBuffer);
#endif
		if (listHead == NULL) {
			listHead = malloc(sizeof(imgData_t));
			curNode = listHead;
		} else {
			prevNode->next = malloc(sizeof(imgData_t));
			curNode = prevNode->next;
		}
		if (!curNode) {
			fprintf(stderr, "Error allocating memory for list node\n");
			return NULL;
		}
		curNode->fileName = strdup(lineBuffer);
		if (!curNode->fileName) {
			fprintf(stderr, "Error copying file name\n");
			return NULL;
		}
		/* Provide sane defaults for all the data fields. */
		curNode->rgbaBufferBig = NULL;
		curNode->rgbaBufferSml = NULL;
		curNode->widthBig = 0;
		curNode->widthSml = 0;
		curNode->heightSml = 0;
		curNode->heightSml = 0;
		curNode->failedProcessing = 0;
		prevNode = curNode;
#ifdef DEBUG_OUTPUT
		printf("Done.\n");
#endif
	}
	/* Free up the line buffer file. */
	fclose(fileList);
	return listHead;
}

/**
 * @name getImageList
 * @brief Should take an image name, read the file in, and decode it into 
 * some kind of pixel array.  
 *
 * See header for details.
 */
void decodeImage(
	const char imgFileName[], 
	char **rgbaArray_p, 
	int *width_p, 
	int *height_p
	) {
	/* File pointer to keep track of the file name. */
	FILE *imgFile; 
	/* Char buffer pointer which we'll fill in. */
	char *imgData_p;
	
	/* Set defaults for the results so we can return if we encounter an error.
	*/
	*rgbaArray_p = NULL;
	*width_p = 0;
	*height_p = 0;
	
	/* Open the image file to determine the type of file it is. */
	imgFile = fopen(imgFileName, "r");
	if (!imgFile) {
		fprintf(stderr, "Error opening image file %s, "
			"check that it exists\n", imgFileName);
		return;
	}
	/* Read in the first 4 bytes. */
	imgData_p = malloc(sizeof(char)*4);
	if (!imgData_p) {
		fprintf(stderr, "Error allocating image data memory\n");
		fclose(imgFile);
		return;
	}
	if (fread(imgData_p, sizeof(char), 4, imgFile) != 4) {
		fprintf(stderr, "Error reading image file first bytes\n");
		free(imgData_p);
		fclose(imgFile);
		return;
	}
	/* Now, check those first 4 bytes to see if we can ID the file type. */
	if (imgData_p[0] == 'B' && imgData_p[1] == 'M') {
		/* This is likely a Bitmap file, let's hand it to the BMP decoder to 
		check. */
		free(imgData_p);
		/* Move back to the start of the file. */
		fseek(imgFile, 0, SEEK_SET);
#ifdef DEBUG_OUTPUT
		/*DEBUG*/printf("bmp decoder used on %s\n", imgFileName);
#endif
		bmpDecode(imgFile, rgbaArray_p, width_p, height_p);
		/* Free up the file. */
		/*fclose(imgFile);*/
		return;
	}
}

/**
 * @name bmpDecode
 * @brief Decodes a BMP file format image into an rbga array
 */
void bmpDecode(FILE *imgFile, char **rgbaArray_p, int *width_p, 
	int *height_p) {
	/* Char buffers to store the header, info block, and image data. */
	char *header_p;
	char *info_p;
	uint32_t *palette_p;
	char *data_p;
	/* Sizes of the file, header, color depth, and data section offset. */
	size_t fileSize, dataOffset, bpc;
	/* Placeholders for the size, until we can be sure we have a valid 
	image. Same for destination buffer. */
	int width, height;
	char *rgbaBuffer;
	/* Variables to store bit depth, compression method, palette size, color 
	table flags. */
	int bpp, compressionMethod, colorPaletteSize, colorPalettePresent;
	/* Provide sane defaults so that if we exit early the caller can tell an 
	error occured. */
	*rgbaArray_p = NULL;
	*width_p = 0;
	*height_p = 0;

	/* First step is to get the other useful info out of the header so we can
	begin decoding. */
	header_p = malloc(sizeof(char) * 14);
	if (!header_p) {
		fprintf(stderr, "Error allocating BMP header memory\n");
		return;
	}
	if (fread(header_p, sizeof(char), 14, imgFile) != 14) {
		fprintf(stderr, "Error reading BMP image file header\n");
		free(header_p);
		return;
	}
	/* Extract important info from the header. */
	fileSize = *(uint32_t *)(header_p + 2);
	fileSize = fileSize; /* Stave off unused variable warnings. */
	dataOffset = *(uint32_t *)(header_p + 10);
	/* free the header pointer now that we're done with it. */
	free(header_p);
	/* Now, we will attempt to detect which info block we're using and read 
	the info out of it. The first 4 bytes determine the info size, and hence
	the format. */
	info_p = malloc(sizeof(char) * 4);
	if (!info_p) {
		fprintf(stderr, "Error allocating BMP info start memory\n");
		return;
	}
	if (fread(info_p, sizeof(char), 4, imgFile) != 4) {
		fprintf(stderr, "Error reading BMP image info block start\n");
		free(info_p);
		return;
	}
	/* The first 4 bytes are the size (as a 32-bit int), and determine the 
	kind of header. */
	switch (*(uint32_t *)(info_p)) {
	case 40:
		/* 40-byte BITMAPINFOHEADER, read the other 36 bytes.  */
		free(info_p);
		info_p = malloc(sizeof(char) * 36);
		if (!info_p) {
			fprintf(stderr, "Error allocating BMP info memory\n");
			return;
		}
		if (fread(info_p, sizeof(char), 36, imgFile)  != 36) {
			fprintf(stderr, "Error reading image BMP info\n");
			free(info_p);
			return;
		}
		/* Get the image dimensions. */
		width = *(uint32_t *)(info_p);
		height = *(uint32_t *)(info_p + 4);
		/* If the height is negative, it means the image is encoded upside 
		down, that's OK, we'll take care of it. */
		if (width < 0) { width *= -1; }
		/* Bit depth and color depth. */
		bpp = *(uint16_t *)(info_p + 10);
		bpc = sizeof(uint32_t);
		/* Check to make sure a supported compression method is used. */
		switch (*(uint32_t *)(info_p + 12)) {
		case 0:
			/* No compression. */
			compressionMethod = 0;
			break;
		default:
			/* Unknown / unsupported compression method. */
			fprintf(stderr, "Unsupported compression method: %d\n",
				(*(uint32_t *)(info_p + 12)));
			free(info_p);
			return;
		}
		/* Number of entries in the color palette. */
		colorPaletteSize = *(uint32_t *)(info_p + 28);
		/* If it was zero, then that means we default to the the max size for 
		the given depth.*/
		colorPaletteSize= colorPaletteSize ? colorPaletteSize : intPow(2,bpp);
		/* Free the info header now that we're done with it. */
		free(info_p);
#ifdef DEBUG_OUTPUT
		/* DEBUG: Displays the info we've gathered. */
		printf("Img info:\nWidth: %d Height %d Depth %d\n"
			"CompressionMethod %d Palette size: %d\n"
			"Color Size: %.8X Header size: %d, Data pos: %.8X\n", width, height, 
			bpp, compressionMethod, colorPaletteSize, 
			(unsigned int)bpc, 40, (unsigned int)dataOffset);
#endif

		/* Now that we have all the critical info, we can read the color 
		palette in. That is, if it exists. If it doesn't, then we need to
		make sure we don't use it. */
		if (dataOffset != 54) {
			palette_p = malloc(bpc*colorPaletteSize);
			if (!palette_p) {
				fprintf(stderr, 
					"Error allocating palette memory\n");
				return;
			}
			if (fread(palette_p, bpc, colorPaletteSize, imgFile) 
				!= (size_t)colorPaletteSize) {
				fprintf(stderr, 
					"Error reading BMP palette data\n");
				free(palette_p);
				return;
			}
			/* DEBUG: For reference, we'll print out the first 16 entries of 
			the color palette. */
			int colCount;
#ifdef DEBUG_OUTPUT
			printf("Color Palette: \n");
			for (colCount = 0; colCount < 16; colCount++) {
				uint32_t color = *(uint32_t *)(palette_p + 
					colCount * sizeof(uint32_t));
				printf("%d-R:%d-G:%d-B%d\n", colCount, (color >> 0) & 0xFF, 
					(color >> 8) & 0xFF, (color >> 16) & 0xFF);
			}
#endif
			colorPalettePresent = 1;
			/* Seek to make sure we're really at the data now.*/
			fseek(imgFile, dataOffset, SEEK_SET);
		} else {
			/* WELP: Turns out there is no color table in this image for some 
			reason, but as long as we're at 16 BPP or higher that's OK. */
			if (bpp <= 16) {
				fprintf(stderr, 
					"BMP decoding Error: No color table for"
					" indexed bitmap.");
				return;
			}
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("No color palette detected!\n");
#endif
			/* Seek to make sure we're really at the data now.*/
			fseek(imgFile, dataOffset, SEEK_SET);
			colorPalettePresent = 0;
		}
		/* Now we can get to the actual decoding. We calculate the row size and
		then read in as many rows as the image is tall. */
		size_t rowsize = ((bpp * width + 31) / 32) * 4;
		data_p = malloc(sizeof(char)*rowsize*height);
		if (!data_p) {
			fprintf(stderr, "Error allocating BMP data memory\n");
			return;
		}
		if (fread(data_p, sizeof(char) * rowsize, height, imgFile) != 
			(size_t)height) {
				fprintf(stderr, "Error reading BMP data\n");
				free(data_p);
				free(palette_p);
				return;
		}
		switch (bpp) {
		case 24:
			/* 24 bits per pixel, format is R8 G8 B8 (red being the LSByte and
			green being the MSByte. */
			/* Finally, we can actually allocate our destination buffer. The 
			format there is R8 G8 B8 A8. */
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Begin decode...");
#endif
			rgbaBuffer = malloc(sizeof(char) * 4 * width*height);
			if (!rgbaBuffer) {
				fprintf(stderr, "Error allocating memory\n");
				free(data_p);
				free(palette_p);
				return;
			}
			int rowOffset, pxOffset;
			/* Iterate through each row in the image. */
			for (rowOffset = 0; rowOffset < height; rowOffset++) {
				/* Iterate through each pixel in the image. */
				for (pxOffset = 0; pxOffset < width; pxOffset++) {
					/* RED */
					rgbaBuffer[rowOffset * width * 4 + pxOffset * 4 + 0] =
						data_p[rowOffset * rowsize + pxOffset * 3 + 2];
					/* GREEN */
					rgbaBuffer[rowOffset * width * 4 + pxOffset * 4 + 1] =
						data_p[rowOffset * rowsize + pxOffset * 3 + 1];
					/* BLUE */
					rgbaBuffer[rowOffset * width * 4 + pxOffset * 4 + 2] =
						data_p[rowOffset * rowsize + pxOffset * 3 + 0];
					/* ALPHA */
					rgbaBuffer[rowOffset * width * 4 + pxOffset * 4 + 3] =
						0xFF; /* No alpha data in image. */
				}
			}
			/* The decoding was successful! We can now assign all the output 
			values and return! */
			*rgbaArray_p = rgbaBuffer;
			*width_p = width;
			*height_p = height;
			/* And finally, free up all those resources we're using. */
			free(data_p);
			if (colorPalettePresent == 1) {
				free(palette_p);
			}
#ifdef DEBUG_OUTPUT
			/*DEBUG*/printf("Done\n");
#endif
			break;
		default:
			fprintf(stderr, "BMP Decoding: Non-supported bit depth mode\n");
			free(data_p);
			free(palette_p);
			return;
		}
		break;

	default:
		/* Unknown / unsupported header type. */
		fprintf(stderr, "Unknown BMP Info block type\n");
		free(info_p);
		return;
	}
	
}

/**
 * @name intPow
 * @brief Short and sweet exponentiation implementation to avoid math.h 
 * dependency for this library. 
 */
int intPow(int n, int exponent) {
	return (exponent ? n * intPow(n, exponent - 1) : 1);
}

/**
	@name testext
	@brief Tests a filename to see if it ends in the given extension. 
*/
int testext(char *filename, char *ext)
{
        char *nameend = filename + (strlen(filename) - strlen(ext));
        return (strcmp(nameend, ext) ? 0 : 1);
}
