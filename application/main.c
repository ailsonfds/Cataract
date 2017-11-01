#include "image_manip.h"

double ** matriz_filtro();
double ** matriz_filtro_gaussiano(double desvio);

int main(int argc, char **argv){
	int i;
	FILE *input_img = fopen(argv[1], "rb");
	FILE *output_img;
	PPMFILEHEADER ppm_header;
	RGBQUAD **matriz;
	/*RGBQUAD **bw_matriz;*/
	RGBQUAD **gauss_matriz;
	double **gauss_filter_m = matriz_filtro_gaussiano(3.0);
	char outstr[50] = "";

	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_bw.ppm");

	if(input_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	read_header_PPM(input_img, &ppm_header);
	read_pixels(&matriz, ppm_header.height, ppm_header.width, input_img, ppm_header.offset);
	fclose(input_img);

	/*bw_matriz = bw_transform(matriz, ppm_header.height, ppm_header.width);*/
	/*gauss_matriz = gauss_filter(matriz, ppm_header.height, ppm_header.width);*/
	gauss_matriz = filter(matriz, ppm_header.height, ppm_header.width, gauss_filter_m, 5);

	/*output_img = fopen(outstr,"wb");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(bw_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);*/

	for(i = 0; i < 50; i++){
		outstr[i] = '\0';
	}
	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_gauss.ppm");

	output_img = fopen(outstr,"wb");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(gauss_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);



	return EXIT_SUCCESS;
}

double ** matriz_filtro(){
    int i, j;
    double a00 = 1.0, a10 = 4.0, a20 = 7.0, a11 = 16.0, a21 = 26.0, a22 = 41.0;
    int cnst = 273;
   
    double **matriz = malloc(5*sizeof(double*));
    for(i = 0; i < 5; i++){
        matriz[i] = malloc(5*sizeof(double));
        for (j = 0; j < 5; j++){
        	if((i == 0 && j == 0) || (i == 0 && j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4))
            	matriz[i][j] = a00/cnst;
            else if((i == 0 && j == 1) || (i == 0 && j == 3) || (i == 1 && j == 0) || (i == 1 && j == 4)
            		|| (i == 3 && j == 1) || (i == 3 && j == 4) || (i == 4 && j == 1) || (i == 4 && j == 3))
            	matriz[i][j] = a10/cnst;
            else if((i == 0 && j == 2) || (i == 2 && j == 0) || (i == 2 && j == 4) || (i == 4 && j == 2))
            	matriz[i][j] = a20/cnst;
            else if((i == 1 && j == 1) || (i == 3 && j == 1) || (i == 3 && j == 3) || (i == 1 && j == 3))
            	matriz[i][j] = a11/cnst;
            else if((i == 1 && j == 2) || (i == 2 && j == 1) || (i == 2 && j == 3) || (i == 3 && j == 2))
            	matriz[i][j] = a21/cnst;
            else
            	matriz[i][j] = a22/cnst;
        }
    }
    return matriz;
}

double ** matriz_filtro_gaussiano(double desvio){
    int i, j, height = 5, width = 5;
	double** gauss = (double**)calloc(height, sizeof(double*));

    for(i = 0; i < height; i++){
        gauss[i] = (double*)calloc(width, sizeof(double));
        for(j = 0; j < width; j++){
        	gauss[i][j] = 1;
        }
    }


    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
			gauss[i][j] = pow(E,(-1*(pow(i,2)+pow(j,2)))/2.0*pow(desvio,2))/(pow(desvio,2)*2.0*PI);
        }
    }

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
			printf("%.3f\t",gauss[i][j]);
        }
        printf("\n");
    }

	return gauss;
}
