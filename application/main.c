#include "image_manip.h"

double ** matriz_filtro(int ordem, double desvio);
double ** matriz_filtro_gaussiano(double desvio);

int main(int argc, char **argv){
	int i, j, r;
	FILE *input_img;
	FILE *output_img;
	PPMFILEHEADER ppm_header;
	RGBQUAD **matriz;
	RGBQUAD **bw_matriz;
	RGBQUAD **gauss_matriz;
	RGBQUAD **sobel_matriz;
	int*** matriz_centers;
	double **gauss_filter_m;
	int gauss_ordem = 5;
	char outstr[50] = "";

	input_img = fopen(argv[1], "rb");

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
	gauss_filter_m = matriz_filtro(gauss_ordem, 1);
	gauss_matriz = conv_filter(bw_matriz, ppm_header.height, ppm_header.width, gauss_filter_m, gauss_ordem);
	/*gauss_matriz = conv_filter(gauss_matriz, ppm_header.height, ppm_header.width, gauss_filter_m, 5);*/
	sobel_matriz = sobel_filter(gauss_matriz, ppm_header.height, ppm_header.width);
	/*sobel_matriz = sobel_filter(sobel_matriz, ppm_header.height, ppm_header.width);*/
	sobel_matriz = threshold(sobel_matriz, ppm_header.height, ppm_header.width);

	output_img = fopen(outstr,"w");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(bw_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);

	for(i = 0; i < 50; i++){
		outstr[i] = '\0';
	}
	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_gauss.ppm");

	output_img = fopen(outstr,"w");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(gauss_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);

	for(i = 0; i < 50; i++){
		outstr[i] = '\0';
	}
	strncpy(outstr, argv[1], strlen(argv[1])-4);
	strcat(outstr, "_sobel.ppm");

	output_img = fopen(outstr,"w");

	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return 0;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(sobel_matriz, ppm_header.height, ppm_header.width, output_img, ppm_header.offset);
	fclose(output_img);

	/*matriz_centers = circle_detection(sobel_matriz, ppm_header.height, ppm_header.width, min(ppm_header.height,ppm_header.width)/2, min(ppm_header.height,ppm_header.width)*0.10);

	for (r = 0; r < max(ppm_header.height,ppm_header.width)/2; r++){
		for (i = 0; i < ppm_header.height; i++){
			for (j = 0; j < ppm_header.width; j++){
				printf("%d ", matriz_centers[i][j][r]);
			}
		}

		printf("%f\n", r + min(ppm_header.height,ppm_header.width)*0.10);
	}*/

	return EXIT_SUCCESS;
}

double ** matriz_filtro(int ordem, double desvio){
    int x, y;
    double** gauss = (double**)calloc(ordem, sizeof(double*));

    for(x = 0 - (ordem/2); x <= ordem/2; x++){
        gauss[x + (ordem/2)] = (double*)calloc(ordem, sizeof(double));
        if (gauss[x + (ordem/2)] == NULL) abort();
        for(y = 0 - (ordem/2); y <= ordem/2; y++){
			gauss[x + (ordem/2)][y + (ordem/2)] = pow(E,(x*x + y*y)/(-2.0*desvio*desvio))/(desvio*desvio*2.0*PI);
        }
    }

	/*for(x = 0; x < ordem; x++){
        for(y = 0; y < ordem; y++){
			printf("a%d%d = %.4f\t", x, y, gauss[x][y]);
        }
        printf("\n");
    }*/

    return gauss;
}

double ** matriz_filtro_gaussiano(double desvio){
    int x, y, height = 5, width = 5;
	double** gauss = (double**)calloc(height, sizeof(double*));

    for(x = -height/2; x <= height/2; x++){
        gauss[x] = (double*)calloc(width, sizeof(double));
        for(y = -width/2; y <= width/2; y++){
			gauss[x][y] = pow(E,(x*x + y*y)/(-2.0*desvio*desvio))/(desvio*desvio*2.0*PI);
        }
    }

	return gauss;
}
