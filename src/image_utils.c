#include "image_utils.h"

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
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*file_header).bfReserved1, sizeof(file_header->bfReserved1), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*file_header).bfReserved2, sizeof(file_header->bfReserved2), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*file_header).bfOffBits, sizeof(file_header->bfOffBits), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biSize, sizeof(info_header->biSize), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biWidth, sizeof(info_header->biWidth), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biHeight, sizeof(info_header->biHeight), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biPlanes, sizeof(info_header->biPlanes), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biBitCount, sizeof(info_header->biBitCount), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biCompression, sizeof(info_header->biCompression), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biSizeImage, sizeof(info_header->biSizeImage), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biXPelsPerMeter, sizeof(info_header->biXPelsPerMeter), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biYPelsPerMeter, sizeof(info_header->biYPelsPerMeter), 1, img);
    if(eoftrash == EOF) abort();
    eoftrash = fread(&(*info_header).biClrUsed, sizeof(info_header->biClrUsed), 1, img);
    if(eoftrash == EOF) abort();
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
    if(eoftrash == EOF) abort();
    if(!(strcmp(header->type, "P6") == 0 || strcmp(header->type, "P3") == 0)){
        printf("Not a ppm valid format!!\n");
        abort();
    }
    get_comment_ppm(img);
    eoftrash = fscanf(img, "%u %u %u", &(*header).width, &(*header).height, &(*header).range);
    fgetc(img);
    if(eoftrash == EOF) abort();
    (*header).offset = ftell(img);
    printf("%s\n%d %d\n%d\n%p\n", header->type, header->width, header->height, header->range, header->offset);
}

void write_header_PPM(FILE *img, PPMFILEHEADER header){
    rewind(img);
    fprintf(img, "%s\n", header.type);
    fprintf(img, "%u %u\n%u\n", header.width, header.height, header.range);
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
    if (strcmp(type,"P3") == 0){
        for(i = 0; i < height; i++){
            (*matriz)[i]=(RGBQUAD*)malloc(width*sizeof(RGBQUAD));
            for(j = 0; j < width; j++){
                (*matriz)[i][j].rgbReserved = fscanf(img,"%c\n%c\n%c\n",&(*matriz)[i][j].rgbRed, &(*matriz)[i][j].rgbGreen, &(*matriz)[i][j].rgbBlue);
            }
        }
    }
    else if (strcmp(type,"P6") == 0){
        for(i = 0; i < height; i++){
            (*matriz)[i]=(RGBQUAD*)malloc(width*sizeof(RGBQUAD));
            for(j = 0; j < width; j++){
                (*matriz)[i][j].rgbReserved = fscanf(img,"%c%c%c",&(*matriz)[i][j].rgbRed, &(*matriz)[i][j].rgbGreen, &(*matriz)[i][j].rgbBlue);
            }
        }
    }
    else if(strcmp(type,"BM") == 0){
        for(i = 0; i < height; i++){
            (*matriz)[i]=(RGBQUAD*)malloc(width*sizeof(RGBQUAD));
            for(j = 0; j < width; j++){
                (*matriz)[i][j].rgbReserved = fscanf(img,"%c%c%c",&(*matriz)[i][j].rgbRed, &(*matriz)[i][j].rgbGreen, &(*matriz)[i][j].rgbBlue);
            }
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

RGBQUAD** copy(RGBQUAD **matriz, int height, int width){
    int i, j;
    RGBQUAD** new_image = (RGBQUAD**)calloc(height, sizeof(RGBQUAD*));
    for(i = 0; i < height; i++){
        new_image[i] = (RGBQUAD*)calloc(width, sizeof(RGBQUAD));
        for(j = 0; j < width; j++){
            new_image[i][j].rgbRed = matriz[i][j].rgbRed;
            new_image[i][j].rgbBlue = matriz[i][j].rgbBlue;
            new_image[i][j].rgbGreen = matriz[i][j].rgbGreen;
        }
    }
    return new_image;
}

void get_comment_ppm(FILE *img){
    char is_hash = fgetc(img);
    char trash[200];
    char *read_success;

    if (is_hash == '\n'){
        is_hash = fgetc(img);
    }

    while(is_hash == '#'){
        read_success = fgets(trash, 200, img);
        if(read_success == NULL) abort();
        is_hash = fgetc(img);
    }
    fseek(img, -1, SEEK_CUR);
}
