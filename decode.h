/*
 * decode.h
 * Developer: Daniel Selmes
 * Student #: 12545253
 * Generates a list of files to be decoded and decodes them from thier native
 * file format into a pixel array. 
 * 
 * Main tasks:
 * 
 * 	Image list handler: Should take a directory and get a list of images to 
 * 		be processed. 
 * 	
 * 	Image decoder: Should take an image name, read the file in, and decode it
 * 		into some kind of pixel array. 
*/

/* Include Guard. */
#ifndef ENCODE_H
#define ENCODE_H

void getImageList();

void decodeImage();

#endif
