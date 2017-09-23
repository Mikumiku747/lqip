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
/* fopen, fread, flcose, printf, fprintf, stderr */
#include <stdlib.h>
/* malloc */
#include <stdint.h>
/* Fixed width integer types (uint8_t, uint32_t, size_t, etc.) */
 
/* Function declarations for internal functions. */
void bmpDecode(FILE *imgFile, char **rgbaArray_p, int *width_p, int *height_p);
int intPow(int n, int exponent);
 
/**
 * @name getImageList
 * @brief Should take a directory and get a list of images to be processed. 
 *
 * See header for details.
 */
void getImageList(void);

/**
 * @name getImageList
 * @brief Should take an image name, read the file in, and decode it into some 
 * kind of pixel array.  
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
	
	/* Set defaults for the results so we can return if we encounter an error. */
	*rgbaArray_p = NULL;
	*width_p = NULL;
	*height_p = NULL;
	
	/* Open the image file to determine the type of file it is. */
	imgFile = fopen(imgFileName, "r");
	if (!imgFile) {
		fprintf(stderr, "Error reading image file %s\n", imgFileName);
		return;
	}
	/* Read in the first 4 bytes. */
	imgData_p = malloc(sizeof(char)*4);
	if (!imgData_p) {
		fprintf(stderr, "Error allocating memory\n");
		fclose(imgFile);
		return;
	}
	if (fread(imgData_p, sizeof(char), 4, imgFile) != 4) {
		fprintf(stderr, "Error reading image file\n");
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
		printf("bmp decoder used on %s\n", imgFileName);
		bmpDecode(imgFile, rgbaArray_p, width_p, height_p);
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
	/* Sizes of the file, header, and data section offset. */
	size_t fileSize, dataOffset;
	/* Placeholders for the size, until we can be sure we have a valid image. */
	int width, height;
	/* Variables to store color depth, compression method, palette size. */
	int bpp, compressionMethod, colorPaletteSize;
	/* Provide sane defaults so that if we exit early the caller can tell an 
	error occured. */
	*rgbaArray_p = NULL;
	*width_p = NULL;
	*height_p = NULL;

	/* First step is to get the other useful info out of the header so we can
	begin decoding. */
	header_p = malloc(sizeof(char) * 14);
	if (!header_p) {
		fprintf(stderr, "Error allocating memory\n");
		return;
	}
	if (fread(header_p, sizeof(char), 14, imgFile) != 14) {
		fprintf(stderr, "Error reading image file\n");
		free(header_p);
		return;
	}
	/* Extract important info from the header. */
	fileSize = *(uint32_t *)(header_p + 2);
	dataOffset = *(uint32_t *)(header_p + 10);
	/* free the header pointer now that we're done with it. */
	free(header_p);
	/* Now, we will attempt to detect which info block we're using and read 
	the info out of it. The first 4 bytes determine the info size, and hence
	the format. */
	info_p = malloc(sizeof(char) * 4);
	if (!info_p) {
		fprintf(stderr, "Error allocating memory\n");
		return;
	}
	if (fread(info_p, sizeof(char), 4, imgFile) != 4) {
		fprintf(stderr, "Error reading image file\n");
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
			fprintf(stderr, "Error allocating memory\n");
			return;
		}
		if (fread(info_p, sizeof(char), 36, imgFile)  != 36) {
			fprintf(stderr, "Error reading image file\n");
			free(info_p);
			return;
		}
		/* Get the image dimensions. */
		width = *(uint32_t *)(info_p);
		height = *(uint32_t *)(info_p + 4);
		bpp = *(uint16_t *)(info_p + 10);
		/* Check to make sure a supported compression method is used. */
		switch (*(uint32_t *)(info_p + 12)) {
		case 0:
			/* No compression. */
			compressionMethod = 0;
			break;
		default:
			/* Unknown / unsupported compression method. */
			fprintf(stderr, "Unsupported compression method\n");
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
		/* DEBUG: Displays the info we've gathered. */
		printf("Img info:\nWidth: %d Height %d Depth %d\n"
			"CompressionMethod %d Palette size: %d\n", width, height, bpp, 
			compressionMethod, colorPaletteSize);

		/* Now that we have all the critical info, we can read the color 
		palette in. */
		palette_p = malloc(sizeof(uint32_t)*colorPaletteSize);
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
 * dependency. 
 */
int intPow(int n, int exponent) {
	return (exponent ? n * intPow(n, exponent - 1) : 1);
}
