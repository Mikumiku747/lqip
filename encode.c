/**
 * @file: encode.c
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Daniel Selmes  @student_id: 12545253
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @TODO:
 *
 */

/* Includes. */
#include "encode.h"

#include <stdlib.h>
/* malloc, free. */
#include <stdio.h>
/* fprintf */
#include <string.h>

/* This is the header data that is required to be on the front of the BMP
image. */
char bmpHeaderData[] = {
	0x42, 0x4D, 				/* Magic marker. */
	0x00, 0x00, 0x00, 0x00, 	/* File Size. */
	0x00, 0x00, 				/* Reserved 1. */
	0x00, 0x00, 				/* Reserved 2. */
	0x36, 0x00, 0x00, 0x00, 	/* Starting of image data. */
	
	0x28, 0x00, 0x00, 0x00, 	/* Header size. */
	0x00, 0x00, 0x00, 0x00, 	/* Width. */
	0x00, 0x00, 0x00, 0x00, 	/* Height */
	0x01, 0x00, 				/* Color Planes (1). */
	0x20, 0x00, 				/* Color Depth (32). */
	0x00, 0x00, 0x00, 0x00, 	/* Compression Method. */
	0x00, 0x00, 0x00, 0x00, 	/* Image size. */
	0x00, 0x00, 0x00, 0x00, 	/* Horizontal resolution (unused). */
	0x00, 0x00, 0x00, 0x00, 	/* Vertical resolution (unused). */
	0x00, 0x00, 0x00, 0x00, 	/* Palette size (unused). */
	0x00, 0x00, 0x00, 0x00		/* 'Important' colors (unused). */
};

/**
 * @name encodeImageBmp
 * @brief Takes an RGBA array and encodes it as a BMP image file 
 *
 * See header for more information. 
 */
char *encodeImageBmp(const char *rgbaArray, int width, int height) {
	/* First, try to allocate the array. */
	char *bmpBuffer = malloc(sizeof(char)*54 + sizeof(char)*width*height*4);
	if (!bmpBuffer) {
		fprintf(stderr, "Error allocating BMP buffer.\n");
		return NULL;
	}
	/* Add the header to the start. */
	memcpy(bmpBuffer, bmpHeaderData, sizeof(char)*54);
	/* Fill in the various fields of the header. */
	*(int *)(bmpBuffer + 02) = (width * height * 4) + 54; /* File size. */
	*(int *)(bmpBuffer + 18) = width;	/* image width & height. */
	*(int *)(bmpBuffer + 22) = height;
	*(int *)(bmpBuffer + 34) = (width * height * 4);
	/* Copy in the image data. */
	memcpy(bmpBuffer + 54, rgbaArray, sizeof(char)*width*height*4);
	/* Perform color correction (swap G and B). */
	int pxIndex;
	for (pxIndex = 0; pxIndex < width*height; pxIndex++) {
		char red = bmpBuffer[54 + pxIndex*4 + 0];
		char grn = bmpBuffer[54 + pxIndex*4 + 1];
		char blu = bmpBuffer[54 + pxIndex*4 + 2];
		char alp = bmpBuffer[54 + pxIndex*4 + 3];
		bmpBuffer[54 + pxIndex*4 + 0] = blu;
		bmpBuffer[54 + pxIndex*4 + 1] = grn;
		bmpBuffer[54 + pxIndex*4 + 2] = red;
		bmpBuffer[54 + pxIndex*4 + 3] = alp;
	}
	/* Return the completed buffer. */
	return bmpBuffer;
}
