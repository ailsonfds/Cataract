#include "image_manip.h"

void read_header_BMP(FILE *img, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *info_header){	
    int eoftrash;
    strcpy(file_header->bfType, "");
    rewind(img);
    eoftrash = fread(&(*file_header).bfType, sizeof(file_header->bfType)-1, 1, img);
    if(strcmp(file_header->bfType, "BM") != 0){
        printf("Not a bmp format!!\n");
        abort();
    }
    eoftrash = fread(&(*file_header).bfSize, sizeof(file_header->bfSize), 1, img);
    eoftrash = fread(&(*file_header).bfReserved1, sizeof(file_header->bfReserved1), 1, img);
    eoftrash = fread(&(*file_header).bfReserved2, sizeof(file_header->bfReserved2), 1, img);
    eoftrash = fread(&(*file_header).bfOffBits, sizeof(file_header->bfOffBits), 1, img);
    eoftrash = fread(&(*info_header).biSize, sizeof(info_header->biSize), 1, img);
    eoftrash = fread(&(*info_header).biWidth, sizeof(info_header->biWidth), 1, img);
    eoftrash = fread(&(*info_header).biHeight, sizeof(info_header->biHeight), 1, img);
    eoftrash = fread(&(*info_header).biPlanes, sizeof(info_header->biPlanes), 1, img);
    eoftrash = fread(&(*info_header).biBitCount, sizeof(info_header->biBitCount), 1, img);
    eoftrash = fread(&(*info_header).biCompression, sizeof(info_header->biCompression), 1, img);
    eoftrash = fread(&(*info_header).biSizeImage, sizeof(info_header->biSizeImage), 1, img);
    eoftrash = fread(&(*info_header).biXPelsPerMeter, sizeof(info_header->biXPelsPerMeter), 1, img);
    eoftrash = fread(&(*info_header).biYPelsPerMeter, sizeof(info_header->biYPelsPerMeter), 1, img);
    eoftrash = fread(&(*info_header).biClrUsed, sizeof(info_header->biClrUsed), 1, img);
    eoftrash = fread(&(*info_header).biClrImportant, sizeof(info_header->biClrImportant), 1, img);
    if(eoftrash == EOF) abort();

}

void write_header_BMP(FILE *img, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header){
	rewind(img);
	fwrite(&file_header.bfType, sizeof(file_header.bfType)-1, 1,img);
    fwrite(&file_header.bfSize, sizeof(file_header.bfSize), 1,img);
    fwrite(&file_header.bfReserved1, sizeof(file_header.bfReserved1), 1,img);
    fwrite(&file_header.bfReserved2, sizeof(file_header.bfReserved2), 1,img);
    fwrite(&file_header.bfOffBits, sizeof(file_header.bfOffBits), 1,img);
    fwrite(&info_header.biSize, sizeof(info_header.biSize), 1,img);
    fwrite(&info_header.biWidth, sizeof(info_header.biWidth), 1,img);
    fwrite(&info_header.biHeight, sizeof(info_header.biHeight), 1,img);
    fwrite(&info_header.biPlanes, sizeof(info_header.biPlanes), 1,img);
    fwrite(&info_header.biBitCount, sizeof(info_header.biBitCount), 1,img);
    fwrite(&info_header.biCompression, sizeof(info_header.biCompression), 1,img);
    fwrite(&info_header.biSizeImage, sizeof(info_header.biSizeImage), 1,img);
    fwrite(&info_header.biXPelsPerMeter, sizeof(info_header.biXPelsPerMeter), 1,img);
    fwrite(&info_header.biYPelsPerMeter, sizeof(info_header.biYPelsPerMeter), 1,img);
    fwrite(&info_header.biClrUsed, sizeof(info_header.biClrUsed), 1,img);
    fwrite(&info_header.biClrImportant, sizeof(info_header.biClrImportant), 1,img);
}

void read_header_PPM(FILE *img, PPMFILEHEADER *header){
    int eoftrash;
    rewind(img);
    eoftrash = fscanf(img, "%s", (*header).type);
    if(!(strcmp(header->type, "P6") == 0 || strcmp(header->type, "P3") == 0)){
        printf("Not a ppm valid format!!\n");
        abort();
    }
    eoftrash = fscanf(img, "%u %u %u", &(*header).height, &(*header).width, &(*header).range);
    fgetc(img);
    if(eoftrash == EOF) abort();
    (*header).offset = ftell(img);
}

void write_header_PPM(FILE *img, PPMFILEHEADER header){
    rewind(img);
    fprintf(img, "%s\n", header.type);
    fprintf(img, "%u %u\n%u\n", header.height, header.width, header.range);
}

void read_pixels(RGBQUAD ***matriz, int height, int width, FILE *img, int offset){
    int i, j;
    char type[3];
    strcpy(type, "");
    rewind(img);
    i = fscanf(img, "%c%c", &type[0], &type[1]);
    type[2] = '\0';
    rewind(img);
    fseek(img, offset, SEEK_SET);
    *matriz=(RGBQUAD**)malloc(height*sizeof(RGBQUAD*));
    for(i = 0; i < height; i++){
        (*matriz)[i]=(RGBQUAD*)malloc(width*sizeof(RGBQUAD));
        for(j = 0; j < width; j++){
            (*matriz)[i][j].rgbReserved = fscanf(img,"%c%c%c",&(*matriz)[i][j].rgbRed, &(*matriz)[i][j].rgbGreen, &(*matriz)[i][j].rgbBlue);
        }
        if(strcmp(type,"BM") == 0){
            j = fread(&(*matriz)[i][j].rgbReserved, width%4, 1, img);
        }
    }
}

void write_pixels(RGBQUAD **matriz, int height, int width, FILE *img, int offset){
    int i, j;
    char type[3];
    strcpy(type, "");
    rewind(img);
    i = fscanf(img, "%c%c", &type[0], &type[1]);
    type[2] = '\0';
    rewind(img);
    fseek(img, offset, SEEK_SET);
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            fprintf(img,"%c%c%c",matriz[i][j].rgbRed, matriz[i][j].rgbGreen, matriz[i][j].rgbBlue);
        }
        if(strcmp(type,"BM") == 0){
            fwrite(&matriz[i][j].rgbReserved, width%4, 1, img);
        }
    }
}

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

double mean(int *range, int n){
	int i;
	double media = 0.0;
	for(i = 0; i < n; i++){
		media += range[i];
	}
	return (media*1.0)/(n*1.0);
}

double std_deviation(int *range, int n){
	int i;
	double desvio = 0.0;
	double media = mean(range, n);
	for(i = 0; i < n; i++){
		desvio += pow((media - range[i]),2);
	}
	return sqrt(desvio/((n*1.0)-1));
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
	/*nt rangeRed[25] = {0}, rangeBlue[25] = {0}, rangeGreen[25] = {0};*/
	double media_r = pix_mean(matriz, height, width, 'r'), media_g = pix_mean(matriz, height, width, 'g'), media_b = pix_mean(matriz, height, width, 'b');
    double desvio_r = 10.0, desvio_g = 10.0, desvio_b = 10.0;
    RGBQUAD** gauss = (RGBQUAD**)calloc(height, sizeof(RGBQUAD*));

    /*desvio_r = pix_std_deviation(matriz, height, width, 'r');
    desvio_g = pix_std_deviation(matriz, height, width, 'g');
    desvio_b = pix_std_deviation(matriz, height, width, 'b');*/

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
			/*media = mean(rangeRed, 25);*/
			/*desvio = std_deviation(rangeRed, 25);*/
			gauss[i][j].rgbRed = pow(E,pow(media_r - matriz[i][j].rgbRed,2)/(2.0*desvio_r))/(desvio_r*sqrt(2.0*PI));
			/*media = mean(rangeBlue, 25);*/
			/*desvio = std_deviation(rangeBlue, 25);*/
			gauss[i][j].rgbBlue = pow(E,pow(media_b - matriz[i][j].rgbBlue,2)/(2.0*desvio_b))/(desvio_b*sqrt(2.0*PI));
			/*media = mean(rangeGreen, 25);*/
			/*desvio = std_deviation(rangeGreen, 25);*/
			gauss[i][j].rgbGreen = pow(E,pow(media_g - matriz[i][j].rgbGreen,2)/(2.0*desvio_g))/(desvio_g*sqrt(2.0*PI));
        }
    }
	return gauss;
}

RGBQUAD** filter(RGBQUAD **pix_image, int img_height, int img_width, double **m_filter, int m_size){
    int i, j, a, b;
    double sum_r, sum_g, sum_b;
    RGBQUAD** new_image = (RGBQUAD**)calloc(img_height, sizeof(RGBQUAD*));
    FILE * error_f = fopen("error.txt","w");
    if(error_f == NULL) abort();
    for(i = 0; i < img_height; i++){
        new_image[i] = (RGBQUAD*)calloc(img_width, sizeof(RGBQUAD));
        for(j = 0; j < img_width; j++){
            sum_r = 0.0;
            sum_g = 0.0;
            sum_b = 0.0;
            for(a = 0 - (m_size/2); a < m_size/2; a++){
                for(b = 0 - (m_size/2); b < m_size/2; b++){
                    if(i + a < 0 || j + b < 0 || i + a >= img_height || j + b >= img_width) continue;
                    sum_r = sum_r + m_filter[a + (m_size/2)][b + (m_size/2)] * pix_image[i + a][j + b].rgbRed;
                    sum_g = sum_g + m_filter[a + (m_size/2)][b + (m_size/2)] * pix_image[i + a][j + b].rgbGreen;
                    sum_b = sum_b + m_filter[a + (m_size/2)][b + (m_size/2)] * pix_image[i + a][j + b].rgbBlue;
                }
            }
            new_image[i][j].rgbRed = sum_r;
            new_image[i][j].rgbGreen = sum_g;
            new_image[i][j].rgbBlue = sum_b;
            fprintf(error_f, "%c/%.3f %c/%.3f %c/%.3f\n", new_image[i][j].rgbRed, sum_r, new_image[i][j].rgbGreen, sum_g, new_image[i][j].rgbBlue, sum_b);
        }
    }
    return new_image;
}