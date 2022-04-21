
// user defined functions

#include <iostream>
#include <cmath>

#include <windows.h>

using namespace std;

// include this header file for basic image transfer functions
#include "image_transfer.h"

// include this header file for computer vision functions
#include "vision.h"

#include "my_functions.h"

// calculate the average RGB colour of an object with
// a label image value of label_num
void average_colour(image &rgb, image &label_image, int label_num,
	double &R, double &G, double &B)
{
	int i, j, k, label, N;
	int height, width; // ints are 4 bytes on the PC
	ibyte *p; // pointer to colour components in the rgb image
	i2byte *pl; // pointer to the label image

	height = rgb.height;
	width =  rgb.width;

	p = rgb.pdata;
	pl = (i2byte *)label_image.pdata;

	// initialize the summation varibles to compute average colour
	R = 0.0;
	G = 0.0;
	B = 0.0;
	N = 0; // number of pixels with the label number of interest
/*
	for(j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			// equivalent 1D array index k
			k = j*width + i; // pixel number
			// how to get j and i from k ?
			// i = k % width
			// j = (k - i) / width

			// label value for i,j
			label = pl[k]; // method #1 -- 1D array index
//			label = *(pl+k); // method #2 -- 1D array pointer notation

			// collect data if the pixel has the label of interest
			if (label == label_num) {
				N++;
				// 3 bytes per pixel -- colour in order BGR
				B += p[k*3]; // 1st byte in pixel
				G += p[k*3+1]; // 2nd byte in pixel
				R += p[k*3+2]; // 3rd
			}

		} // for i

	} // for j
*/
	// method #3 -- pointers only !
	for(k=0;k<width*height;k++) { // loop for kth pixel

		// how to get j and i from k ?
		i = k % width;
		j = (k - i) / width;
		label = *pl;

		// collect data if the pixel has the label of interest
		if (label == label_num) {
			N++;
			// 3 bytes per pixel -- colour in order BGR
			B += *p; // 1st byte in pixel
			G += *(p+1); // 2nd byte in pixel
			R += *(p+2); // 3rd
		}

		// increment pointers
		p+=3; // 3 bytes per pixel
		pl++;

	}

	// compute average colour
	R = R / N;
	G = G / N;
	B = B / N;

}

