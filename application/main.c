#include "cataract.h"
#include "args.h"
#include <time.h>


int main(int argc, char **argv){
	FILE *input_img;
	FILE *output_img;
	PPMFILEHEADER ppm_header;
	RGBQUAD **matriz;
	char outstr[50] = "";
	time_t begin_time, end_time;

	input_img = fopen(argv[2], "rb");

	strncpy(outstr, argv[2], strlen(argv[2])-4);
	strcat(outstr, "_bw.ppm");

	if(input_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	read_header_PPM(input_img, &ppm_header);
	read_pixels(&matriz, ppm_header.height, ppm_header.width, input_img, ppm_header.offset);
	if (ppm_header.has_comment == 1){
		output_img = tmpfile();
		write_header_PPM(output_img, ppm_header);
		read_header_PPM(output_img, &ppm_header);
		fclose(output_img);
	}
	fclose(input_img);

	time(&begin_time);
	make_diagnoses(ppm_header, matriz, argv[4]);
	time(&end_time);
	printf("%.f s\n", difftime(end_time,begin_time));


	return EXIT_SUCCESS;
}