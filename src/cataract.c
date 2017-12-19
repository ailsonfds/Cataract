#include "cataract.h"

RGBQUAD** apply_necessary_filters(RGBQUAD** img, int height, int width, char* output_name){
	int gauss_ordem = 7;
	double **gauss_filter_m = matriz_filtro(gauss_ordem, 1);
	RGBQUAD **bw_matriz, **gauss_matriz, **sobel_matriz, **bin_matriz;
    PPMFILEHEADER header;
    FILE *output_img;
    char tmpstr[500] = "";

    header.height = height;
    header.width = width;
    strcpy(header.type,"P6");
    header.range = 255;
    header.has_comment = 0;

    output_img = tmpfile();
    write_header_PPM(output_img, header);
    read_header_PPM(output_img, &header);
    fclose(output_img);


	bw_matriz = bw_transform(img, height, width);

    strncpy(tmpstr,output_name, strlen(output_name)-4);
    tmpstr[strlen(output_name)-4] = '\0';
    strcat(tmpstr,"_bw.ppm");
    output_img = fopen(tmpstr, "w");
    if(output_img != NULL){
        write_header_PPM(output_img, header);
        write_pixels(bw_matriz, height, width, output_img, header.offset);
    }
    fclose(output_img);

	gauss_filter_m = matriz_filtro(gauss_ordem, 2);
	gauss_matriz = conv_filter(bw_matriz, height, width, gauss_filter_m, gauss_ordem);

    strcpy(tmpstr,"\0");
    strncpy(tmpstr,output_name, strlen(output_name)-4);
    tmpstr[strlen(output_name)-4] = '\0';
    strcat(tmpstr,"_gauss.ppm");
    output_img = fopen(tmpstr, "w");
    if(output_img != NULL){
        write_header_PPM(output_img, header);
        write_pixels(gauss_matriz, height, width, output_img, header.offset);
    }
    fclose(output_img);

	sobel_matriz = sobel_filter(gauss_matriz, height, width);

    strcpy(tmpstr,"\0");
    strncpy(tmpstr,output_name, strlen(output_name)-4);
    tmpstr[strlen(output_name)-4] = '\0';
    strcat(tmpstr,"_sobel.ppm");
    output_img = fopen(tmpstr, "w");
    if(output_img != NULL){
        write_header_PPM(output_img, header);
        write_pixels(sobel_matriz, height, width, output_img, header.offset);
    }
    fclose(output_img);

	bin_matriz = threshold(sobel_matriz, height, width);

    strcpy(tmpstr,"\0");
    strncpy(tmpstr,output_name, strlen(output_name)-4);
    tmpstr[strlen(output_name)-4] = '\0';
    strcat(tmpstr,"_bin.ppm");
    output_img = fopen(tmpstr, "w");
    if(output_img != NULL){
        write_header_PPM(output_img, header);
        write_pixels(bin_matriz, height, width, output_img, header.offset);
    }
    fclose(output_img);

	dealloc(bw_matriz, height);
	dealloc(gauss_matriz, height);
	dealloc(sobel_matriz, height);
	return bin_matriz;
}

int* circle_detection(RGBQUAD** img, int height, int width, int max_r, int min_r){
	int i, j, t, r, a = 0, b = 0;
    int *matriz_a;

    matriz_a = (int*)calloc(height*width*max_r, sizeof(int));

    printf("Realizando processo de detecção de círculos\n");
    for(i = min_r; i < height - min_r; i++){
        for(j = min_r; j < width - min_r; j++){
            if (img[i][j].rgbRed == 255){
                for(t = 0 ; t < 360; t++){
                    for(r = min_r; r <= max_r; r++){
                        a = i - r*sin(t*PI/180);
                        b = j - r*cos(t*PI/180);
                        if (a > min_r && b > min_r && a < height - min_r && b < width - min_r){
                            matriz_a[(a*width*max_r)+(b*max_r)+r]++;
                        }
                    }
                }
            }
            printf("%.2f%%\r", 100.0*(i - min_r)/(height - (2*min_r)));
        }
    }
    printf("Concluido\n");
    return matriz_a;
}

void make_votation(RGBQUAD** img, int height, int width, int max_r, int min_r, int* matriz_a, int *x_center, int *y_center, int *r_center){
	int i, j, t, r, a = 0, b = 0, aux = 0;
    int max = 0;

    printf("Realizando análise da detecção para determinar o melhor círculo\n");
    for(r = min_r; r <= max_r; r++){        
        for(i = min_r; i < height - min_r; i++){
            for(j = min_r; j < width - min_r; j++){
                if (matriz_a[(i*width*max_r)+(j*max_r)+r] > max){
                    aux = 0;
                    for (t = 0; t < 36000; t++) {
                        a = i + r*sin(t*PI/18000);
                        b = j + r*cos(t*PI/18000);
                        if (a > 0 && b > 0 && a < height && b < width && img[a][b].rgbRed == 255){
                            aux++;
                        }
                    }
                    if (aux >= 2*PI*r*0.20) {
                        max = matriz_a[(i*width*max_r)+(j*max_r)+r];
                        *x_center = i;
                        *y_center = j;
                        *r_center = r;
                    }
                }
            }
            printf("%.2f%%\r", 100.0*(r - min_r)/((max_r- min_r)));
        }
    }
    printf("Concluido\n");
    free(matriz_a);
}

RGBQUAD** segmented_image(RGBQUAD** img, int height, int width, int r_center, int x_center, int y_center){
	int i, j, a = 0, b = 0;
    RGBQUAD ** segmentado;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (r_center < sqrt(pow(i-x_center,2)+pow(j-y_center,2))) {
                img[i][j].rgbRed = 0;
                img[i][j].rgbBlue = 0;
                img[i][j].rgbGreen = 0;
            }
        }
    }

    segmentado = (RGBQUAD**)calloc((r_center*2)+1,sizeof(RGBQUAD*));
    for(i = x_center - r_center, a = 0; i < x_center + r_center; i++, a++){
        segmentado[a] = (RGBQUAD*)calloc((r_center*2)+1,sizeof(RGBQUAD));
        for(j = y_center - r_center, b = 0; j < y_center + r_center; j++, b++){
            segmentado[a][b].rgbRed = img[i][j].rgbRed;
            segmentado[a][b].rgbBlue = img[i][j].rgbBlue;
            segmentado[a][b].rgbGreen = img[i][j].rgbGreen;
        }
    }

    return segmentado;
}

void make_diagnoses(PPMFILEHEADER ppm_header, RGBQUAD** img, char *img_name, char* output_name){
	int i, j, sum, total;
	double diagnose;
	FILE* output_img, *diagnose_file;
	int height = ppm_header.height, width = ppm_header.width;
	int max_r = min(height,width)*0.50, min_r = min(height,width)*0.10;
	int x_c, y_c, r_c, *matriz_a;
    RGBQUAD** copia_orig = copy(img,height,width);
	RGBQUAD** bin;
    char tmp_name[500] = "images/output";
    if(strncmp(img_name,"images/input", 12) == 0){
        for (i = 11; i < strlen(img_name); ++i){
            tmp_name[i+1] = img_name[i];
        }
        strcpy(img_name,tmp_name);
    }
    bin = apply_necessary_filters(img, height, width, img_name);
	matriz_a = circle_detection(bin, height, width, max_r, min_r);
	make_votation(bin, height, width, max_r, min_r, matriz_a, &x_c, &y_c, &r_c);
	bin = segmented_image(copia_orig, height, width, r_c, x_c, y_c);

    ppm_header.height = r_c*2;
    ppm_header.width = r_c*2;

    output_img = tmpfile();
    write_header_PPM(output_img, ppm_header);
    read_header_PPM(output_img, &ppm_header);
    fclose(output_img);

    strncpy(img_name, tmp_name, strlen(tmp_name) - 4);
    img_name[strlen(output_name)-4] = '\0';
    strcat(img_name,"_seg.ppm");
	output_img = fopen(img_name,"w");
	
	if(output_img == NULL){
		fprintf(stderr, "File not opened!!\n");
		return ;
	}

	write_header_PPM(output_img, ppm_header);
	write_pixels(bin, r_c*2, r_c*2, output_img, ppm_header.offset);
	fclose(output_img);

	sum = 0;
	total = 0;
	for (i = 0; i < r_c*2; i++) {
		for (j = 0; j < r_c*2; j++) {
			if (bin[i][j].rgbRed > 0 && bin[i][j].rgbBlue > 0 && bin[i][j].rgbGreen > 0) {
				total++;
				if (bin[i][j].rgbRed > 255*0.3 && bin[i][j].rgbBlue > 255*0.3 && bin[i][j].rgbGreen > 255*0.3 &&
					bin[i][j].rgbRed < 255*0.97 && bin[i][j].rgbBlue < 255*0.97 && bin[i][j].rgbGreen < 255*0.97) {
					sum++;
				}
			}
		}
	}

    diagnose_file = fopen(output_name,"w");
    
    if(output_img == NULL){
        fprintf(stderr, "File not opened!!\n");
        return ;
    }

	diagnose = 100.0*sum/total;
	fprintf(diagnose_file, "O paciente possui %.2f %% de comprometimento da retina\n", diagnose);
	if (diagnose > 50) {
		fprintf(diagnose_file, "Detectado catarata!\n");
	} else {
		fprintf(diagnose_file, "Não detectado catarata!\n");
	}
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
