#include "image_manip.h"

RGBQUAD** bw_transform(RGBQUAD **matriz, int height, int width){
	int i, j;
    int cinza;
    RGBQUAD** bw_matriz;
    bw_matriz=(RGBQUAD**)malloc(height*sizeof(RGBQUAD*));
    for(i = 0; i < height; i++){
        bw_matriz[i]=(RGBQUAD*)malloc(width*sizeof(RGBQUAD));
        for(j = 0; j < width; j++){
            cinza = (matriz[i][j].rgbRed*0.30) + (matriz[i][j].rgbGreen*0.59) + (matriz[i][j].rgbBlue*0.11);
            bw_matriz[i][j].rgbRed = cinza;
            bw_matriz[i][j].rgbGreen = cinza;
            bw_matriz[i][j].rgbBlue = cinza;
        }
    }
    return bw_matriz;
}

double pix_mean(RGBQUAD **matriz, int height, int width, char rgb){
    int i, j;
    double media = 0.0;
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            if(rgb == 'r')
                media += matriz[i][j].rgbRed;
            else if(rgb == 'g')
                media += matriz[i][j].rgbGreen;
            else if(rgb == 'b')
                media += matriz[i][j].rgbBlue;
        }
    }
    return (media*1.0)/(height*width*1.0);
}

double pix_std_deviation(RGBQUAD **matriz, int height, int width, char rgb){
    int i, j;
    double desvio = 0.0;
    double media = pix_mean(matriz, height, width, rgb);
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            if(rgb == 'r')
                desvio += pow(media - matriz[i][j].rgbRed, 2);
            else if(rgb == 'g')
                desvio += pow(media - matriz[i][j].rgbGreen, 2);
            else if(rgb == 'b')
                desvio += pow(media - matriz[i][j].rgbBlue, 2);
        }
    }
    return sqrt(desvio/((height*width*1.0)-1));
}

RGBQUAD** gauss_filter(RGBQUAD **matriz, int height, int width){
	int i, j/*, a, b*/;
	/*int rangeRed[25] = {0}, rangeBlue[25] = {0}, rangeGreen[25] = {0};*/
	double media_r = pix_mean(matriz, height, width, 'r'), media_g = pix_mean(matriz, height, width, 'g'), media_b = pix_mean(matriz, height, width, 'b');
    double desvio_r = 10.0, desvio_g = 10.0, desvio_b = 10.0;
    RGBQUAD** gauss = (RGBQUAD**)calloc(height, sizeof(RGBQUAD*));

    desvio_r = pix_std_deviation(matriz, height, width, 'r');
    desvio_g = pix_std_deviation(matriz, height, width, 'g');
    desvio_b = pix_std_deviation(matriz, height, width, 'b');

    for(i = 0; i < height; i++){
        gauss[i] = (RGBQUAD*)calloc(width, sizeof(RGBQUAD));
        for(j = 0; j < width; j++){
			/*for(a = -(5/2); a < (5/2); a++){
				for(b = -(5/2); b < (5/2); b++){
					if(i+a >= 0 && j+b >= 0 && i+a < height && j+b < width){
						rangeRed[((a+(5/2))*5)+(b+(5/2))] = matriz[i+a][j+b].rgbRed;
						rangeBlue[((a+(5/2))*5)+(b+(5/2))] = matriz[i+a][j+b].rgbBlue;
						rangeGreen[((a+(5/2))*5)+(b+(5/2))] = matriz[i+a][j+b].rgbGreen;
					}
				}
			}*/
			/*media_r = mean(rangeRed, 25);
			desvio_r = std_deviation(rangeRed, 25);*/
			gauss[i][j].rgbRed = pow(E,pow(media_r - matriz[i][j].rgbRed,2)/(2.0*desvio_r))/(desvio_r*sqrt(2.0*PI));
			/*media_b = mean(rangeBlue, 25);
			desvio_b = std_deviation(rangeBlue, 25);*/
			gauss[i][j].rgbBlue = pow(E,pow(media_b - matriz[i][j].rgbBlue,2)/(2.0*desvio_b))/(desvio_b*sqrt(2.0*PI));
			/*media_g = mean(rangeGreen, 25);
			desvio_g = std_deviation(rangeGreen, 25);*/
			gauss[i][j].rgbGreen = pow(E,pow(media_g - matriz[i][j].rgbGreen,2)/(2.0*desvio_g))/(desvio_g*sqrt(2.0*PI));
        }
    }
	return gauss;
}

RGBQUAD** conv_filter(RGBQUAD **pix_image, int img_height, int img_width, double **m_filter, int m_size){
    int i, j, a, b;
    double sum_r, sum_g, sum_b;
    RGBQUAD** new_image = (RGBQUAD**)calloc(img_height, sizeof(RGBQUAD*));
    /*FILE * error_f = fopen("error.txt","w");
    if(error_f == NULL) abort();*/
    for(i = 0; i < img_height; i++){
        new_image[i] = (RGBQUAD*)calloc(img_width, sizeof(RGBQUAD));
        for(j = 0; j < img_width; j++){
            sum_r = 0.0;
            sum_g = 0.0;
            sum_b = 0.0;
            for(a = 0 - (m_size/2); a <= m_size/2; a++){
                for(b = 0 - (m_size/2); b <= m_size/2; b++){
                    if(i + a >= 0 && j + b >= 0 && i + a < img_height && j + b < img_width){
                        sum_r += (m_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbRed);
                        sum_g += (m_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbGreen);
                        sum_b += (m_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbBlue);
                    }
                }
            }
            if (sum_r > 255) sum_r = 255;
            if (sum_g > 255) sum_g = 255;
            if (sum_b > 255) sum_b = 255;
            new_image[i][j].rgbRed = sum_r;
            new_image[i][j].rgbGreen = sum_g;
            new_image[i][j].rgbBlue = sum_b;
            /*fprintf(error_f, "%c/%.0f %c/%.0f %c/%.0f\n", new_image[i][j].rgbRed, sum_r, new_image[i][j].rgbGreen, sum_g, new_image[i][j].rgbBlue, sum_b);*/
        }
    }
    return new_image;
}

RGBQUAD** sobel_filter(RGBQUAD **pix_image, int img_height, int img_width){
    int g_filter[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
    int i, j, a, b, m_size = 3;
    double sum_r, sum_g, sum_b, sum_r_x, sum_g_x, sum_b_x, sum_r_y, sum_g_y, sum_b_y;
    RGBQUAD** new_image = (RGBQUAD**)calloc(img_height, sizeof(RGBQUAD*));
    /*FILE * error_f = fopen("error.txt","w");
    if(error_f == NULL) abort();*/
    for(i = 0; i < img_height; i++){
        new_image[i] = (RGBQUAD*)calloc(img_width, sizeof(RGBQUAD));
        for(j = 0; j < img_width; j++){
            sum_r = 0.0;
            sum_g = 0.0;
            sum_b = 0.0;
            sum_r_x = 0.0;
            sum_g_x = 0.0;
            sum_b_x = 0.0;
            sum_r_y = 0.0;
            sum_g_y = 0.0;
            sum_b_y = 0.0;
            for(a = 0 - (m_size/2); a <= m_size/2; a++){
                for(b = 0 - (m_size/2); b <= m_size/2; b++){
                    if(i + a >= 0 && j + b >= 0 && i + a < img_height && j + b < img_width){
                        sum_r_x += (g_filter[b + (m_size/2)][a + (m_size/2)]) * (pix_image[i + a][j + b].rgbRed);
                        sum_g_x += (g_filter[b + (m_size/2)][a + (m_size/2)]) * (pix_image[i + a][j + b].rgbGreen);
                        sum_b_x += (g_filter[b + (m_size/2)][a + (m_size/2)]) * (pix_image[i + a][j + b].rgbBlue);
                        sum_r_y += (g_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbRed);
                        sum_g_y += (g_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbGreen);
                        sum_b_y += (g_filter[a + (m_size/2)][b + (m_size/2)]) * (pix_image[i + a][j + b].rgbBlue);
                    }
                }
            }
            sum_r = sqrt(pow(sum_r_y,2) + pow(sum_r_y,2));
            sum_g = sqrt(pow(sum_g_y,2) + pow(sum_g_y,2));
            sum_b = sqrt(pow(sum_b_y,2) + pow(sum_b_y,2));
            new_image[i][j].rgbReserved = atan2(sum_r_y,sum_r_x);
            if (sum_r > 255) sum_r = 255;
            if (sum_g > 255) sum_g = 255;
            if (sum_b > 255) sum_b = 255;
            new_image[i][j].rgbRed = sum_r;
            new_image[i][j].rgbGreen = sum_g;
            new_image[i][j].rgbBlue = sum_b;
            /*fprintf(error_f, "%c/%.0f %c/%.0f %c/%.0f\n", new_image[i][j].rgbRed, sum_r, new_image[i][j].rgbGreen, sum_g, new_image[i][j].rgbBlue, sum_b);*/
        }
    }
    return new_image;
}

RGBQUAD** threshold(RGBQUAD **matriz, int height, int width){
    int i, j;
    /*int thres_i = max_in_range(histogram(matriz, height, width),THRES_RANGE);
    int thres = (256/THRES_RANGE)*thres_i;*/
    double media = pix_mean(matriz, height, width, 'r');
    /*double desvio = pix_std_deviation(matriz, height, width, 'r');*/
    RGBQUAD** new_image = (RGBQUAD**)calloc(height, sizeof(RGBQUAD*));
    for(i = 0; i < height; i++){
        new_image[i] = (RGBQUAD*)calloc(width, sizeof(RGBQUAD));
        for(j = 0; j < width; j++){
            if (matriz[i][j].rgbRed > media*0.75){
                new_image[i][j].rgbRed = 255;
                new_image[i][j].rgbBlue = 255;
                new_image[i][j].rgbGreen = 255;
            }else{
                new_image[i][j].rgbRed = 0;
                new_image[i][j].rgbBlue = 0;
                new_image[i][j].rgbGreen = 0;
            }
        }
    }
    return new_image;
}

int *histogram(RGBQUAD **pix_image, int img_height, int img_width){
    int i, j, k, c;
    int *vet = calloc(THRES_RANGE, sizeof(int));
    for (k = 0; k < THRES_RANGE; k++){
        for (c = 0; c < 256/THRES_RANGE; c++){
            for (i = 0; i < img_height; i++){
                for (j = 0; j < img_height; j++){
                    if (c*k + c == pix_image[i][j].rgbRed || c*k + c == pix_image[i][j].rgbBlue || c*k + c == pix_image[i][j].rgbGreen){
                        vet[k]++;
                    }
                }
            }
        }
    }
    return vet;
}
