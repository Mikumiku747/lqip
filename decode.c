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
/* malloc, free */
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
	
	/* Set defaults for the results so we can return if we encounter an error. 
	*/
	*rgbaArray_p = NULL;
	*width_p = NULL;
	*height_p = NULL;
	
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
		/*DEBUG*/printf("bmp decoder used on %s\n", imgFileName);
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
	unsigned char *rgbaBuffer;
	/* Variables to store bit depth, compression method, palette size, color 
	table flag, row, column. */
	int bpp, compressionMethod, colorPaletteSize, colorPalettePresent, row, 
		col;
	/* Provide sane defaults so that if we exit early the caller can tell an 
	error occured. */
	*rgbaArray_p = NULL;
	*width_p = NULL;
	*height_p = NULL;

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
		/* DEBUG: Displays the info we've gathered. */
		printf("Img info:\nWidth: %d Height %d Depth %d\n"
			"CompressionMethod %d Palette size: %d\n"
			"Color Size: %d Header size: %d, Data pos: %d\n", width, height, 
			bpp, compressionMethod, colorPaletteSize, bpc, 40, dataOffset);

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
				!= colorPaletteSize) {
				fprintf(stderr, 
					"Error reading BMP palette data\n");
				free(palette_p);
				return;
			}
			/* DEBUG: For reference, we'll print out the first 16 entries of 
			the color palette. */
			int colCount;
			printf("Color Palette: \n");
			for (colCount = 0; colCount < 16; colCount++) {
				uint32_t color = *(uint32_t *)(palette_p + 
					colCount * sizeof(uint32_t));
				printf("%d-R:%d-G:%d-B%d\n", colCount, (color >> 0) & 0xFF, 
					(color >> 8) & 0xFF, (color >> 16) & 0xFF);
			}
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
				free(palette_p);
				return;
			}
			/*DEBUG*/printf("No color palette detected!\n");
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
		if (fread(data_p, sizeof(char)*rowsize, height, imgFile) != height) {
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
			/*DEBUG*/printf("Begin decode...");
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
			/*DEBUG*/printf("Done\n");
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
 * dependency. 
 */
int intPow(int n, int exponent) {
	return (exponent ? n * intPow(n, exponent - 1) : 1);
}
