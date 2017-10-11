#include "image_manip.h"

int main(int argc, char **argv){
	FILE *input_img = fopen(argv[1], "rb");
	FILE *output_img;
	PPMFILEHEADER ppm_header;
	RGBQUAD **matriz;
	BWQUAD **bw_matriz;
	char outstr[50] = "";

	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_bw.pgm");

	if(input_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	read_header_PPM(input_img, &ppm_header);
	read_pixels(&matriz, ppm_header.height, ppm_header.width, input_img, ppm_header.offset);
	fclose(input_img);

	strcpy(ppm_header.type,"P5");

	bw_matriz = bw_trasnform(matriz, ppm_header.height, ppm_header.width);

	output_img = fopen(outstr,"wb");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels_PGM(bw_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);

	return EXIT_SUCCESS;
}
