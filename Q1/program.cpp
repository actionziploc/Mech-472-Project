
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

#include "image_transfer.h"

#include "vision.h"

#include "my_functions.h"

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

int main()
{ 
	int nhist,j,nlabels;
	double hist[255],hmin,hmax,x,ic,jc;
	image a,b,rgb,rgb0; // declare some image structures
	image label; // declare a label image (2 bytes/pixel)
	int cam_number, height, width;
	int R, G, B;
	double R_ave, G_ave, B_ave;

	activate_vision();
	
	// set camera number (normally 0 or 1)
	cam_number = 0; 
	width  = 640;
	height = 480;

//	activate_camera(cam_number,height,width);	// activate camera

	rgb0.type = RGB_IMAGE;
	rgb0.width = width;
	rgb0.height = height;

	rgb.type = RGB_IMAGE;
	rgb.width = width;
	rgb.height = height;

	// set the type and size of the images
	a.type = GREY_IMAGE;
	a.width = width;
	a.height = height;

	b.type = GREY_IMAGE;
	b.width = width;
	b.height = height;

	label.type = LABEL_IMAGE;
	label.width = width;
	label.height = height;

	// allocate memory for the images
	allocate_image(a);
	allocate_image(b);
	allocate_image(label);
	allocate_image(rgb);
	allocate_image(rgb0);

	cout << "\npress space key to continue.";
	pause();

	// load an image in RGB format
	
	// * note: load doesn't set rgb parameters or allocate
	// memory automatically as set_rgb_image(...) does
	load_rgb_image("a.bmp",rgb); 

	// copy the original rgb image to use later
	copy(rgb,rgb0);

	view_rgb_image(rgb);
	cout << "\ninput image rgb";
	pause();

	// convert RGB image to a greyscale image
	copy(rgb,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\ninput image converted to greyscale";
	pause();

	// scale the image to enhance contrast
	scale(a,b);
	copy(b,a); // put result back into image a

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage scale function is applied";
	pause();

	save_rgb_image("grey.bmp",rgb);

	// apply a low pass filter to reduce noise
	
	lowpass_filter(a,b);
	copy(b,a);
	
//	lowpass_filter(a,b);
//	copy(b,a);

//	highpass_filter(a,b);
//	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage after filter function is applied";
	pause();

	// make a histogram
	
	nhist = 60; 
	// make 60 bins -- each bin is 255/60 range of intensity
	// eg bin1 = 0-3 
	// bin2 = 4-8,
	// etc.
	histogram(a,hist,nhist,hmin,hmax);

	// save to a csv file you can open/plot with microsoft excel
	
	// * make sure the file is not currently open in excel before 
	// running the program again or the C++ program will not be
	// able to open the file
	ofstream fout("hist1.csv");

	for(j=0;j<nhist;j++) {
		x = hmin + (hmax-hmin)/nhist*j;
		fout << x << "," << hist[j] << "\n";
	}

	fout.close();

	// use threshold function to make a binary image (0,255)
	threshold(a,b,70);
	copy(b,a);

	copy(a,rgb); // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage after threshold function is applied";
	pause();
 
	// invert the image
	invert(a,b);
	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage after invert function is applied";
	pause();

	// perform an erosion function to remove noise (small objects)
	erode(a,b);
	copy(b,a);

//	erode(a,b);
//	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage after erosion function is applied";
	pause();

	// remove dialation for average_colour example since
	// it introduces pixels from the edge of the object that
	// have the colour of the background
	
/*
	// perform a dialation function to fill in 
	// and grow the objects
	dialate(a,b);
	copy(b,a);

	dialate(a,b);
	copy(b,a);
*/

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	cout << "\nimage after dialation function is applied";
	pause();

	// label the objects in a binary image
	// labels go from 1 to nlabels
	label_image(a,label,nlabels);

	// compute the centroid of the last object
	centroid(a,label,2,ic,jc);
	cout << "\ncentroid: ic = " << ic << " jc = " << jc;

	// use the original colour image rgb0 here
	average_colour(rgb0,label,2,R_ave,G_ave,B_ave);

	cout << "\nR_ave = " << R_ave;
	cout << "\nG_ave = " << G_ave;
	cout << "\nB_ave = " << B_ave;

	// convert to RGB image format
	copy(a,rgb);    
	
	// mark the centroid point on the image with a blue point
	R = 0; G = 0; B = 255;
	draw_point_rgb(rgb,(int)ic,(int)jc,R,G,B);
	
	view_rgb_image(rgb);
	cout << "\nimage after a centroid is marked.";
	pause();

	// save the image (make sure to use an RGB image)
	save_rgb_image("aout.bmp",rgb);

	// free the image memory before the program completes
	free_image(a);
	free_image(b);
	free_image(label);
	free_image(rgb);
	free_image(rgb0);
	
	deactivate_vision();

	cout << "\n\ndone.\n";

 	return 0;
}

