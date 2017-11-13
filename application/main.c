#include "image_manip.h"

double ** matriz_filtro();
double ** matriz_filtro_gaussiano(double desvio);

int main(int argc, char **argv){
	int i;
	FILE *input_img = fopen(argv[1], "rb");
	FILE *output_img;
	PPMFILEHEADER ppm_header;
	RGBQUAD **matriz;
	RGBQUAD **bw_matriz;
	RGBQUAD **gauss_matriz;
	RGBQUAD **sobel_matriz;
	int*** matriz_centers;
	double **gauss_filter_m = matriz_filtro();
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

	bw_matriz = bw_transform(matriz, ppm_header.height, ppm_header.width);
	/*gauss_matriz = gauss_filter(bw_matriz, ppm_header.height, ppm_header.width);*/
	gauss_matriz = conv_filter(bw_matriz, ppm_header.height, ppm_header.width, gauss_filter_m, 5);
	/*gauss_matriz = conv_filter(gauss_matriz, ppm_header.height, ppm_header.width, gauss_filter_m, 5);*/
	sobel_matriz = sobel_filter(gauss_matriz, ppm_header.height, ppm_header.width);
	/*sobel_matriz = sobel_filter(sobel_matriz, ppm_header.height, ppm_header.width);*/
	sobel_matriz = threshold(sobel_matriz, ppm_header.height, ppm_header.width);
	matriz_centers = circle_detection(sobel_matriz, height, width, max(height,width)/2, min(height,width)*0.10);

	/*output_img = fopen(outstr,"wb");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(bw_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);*/

	/*for(i = 0; i < 50; i++){
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
	fclose(output_img);*/

	for(i = 0; i < 50; i++){
		outstr[i] = '\0';
	}
	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_sobel.ppm");

	output_img = fopen(outstr,"wb");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(sobel_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);

	for (r = 0; r < max(height,width)/2; r++){
		for (i = 0; i < height; i++){
			for (j = 0; j < width; j++){
				printf("%d ", );
			}
		}

		printf("%d\n", r + min(height,width)*0.10);
	}

	return EXIT_SUCCESS;
}

double ** matriz_filtro(){
    int i, j;
    double a00 = 2.0, a10 = 4.0, a20 = 5.0, a11 = 9.0, a21 = 12.0, a22 = 15.0;
    int cnst = 159;
   
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
    }

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
			gauss[i][j] = pow(E,(pow(i,2)+pow(j,2))/(-2.0)*pow(desvio,2))/(pow(desvio,2)*2.0*PI);
        }
    }

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
			printf("%.15f\t",gauss[i][j]);
        }
        printf("\n");
    }
    printf("\n");

	return gauss;
}
