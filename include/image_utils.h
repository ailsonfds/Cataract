#ifndef __IMAGE_UTILS_H__
#define __IMAGE_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * error_file;

typedef struct tagBITMAPFILEHEADER{  /* ocupa 14 bytes e é o 1° a aparecer no ficheiro */
	char bfType[3]; /* especifica o firmato BM */
	unsigned int bfSize; /* tamanho do arquivo em bytes */
	unsigned short bfReserved1;	
	unsigned short bfReserved2;	
	unsigned int bfOffBits; /* offset da matriz de pixels */
}BITMAPFILEHEADER;

typedef struct tagRGBQUAD{
	unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; /* cores RGB da imagem */
}RGBQUAD;

typedef struct tagBITMPAINFOHEADER{ 
	unsigned int biSize; /* tamanho do cabeçalho */
	int biWidth; /* largura da imagem */
	int biHeight; /* altura da imagem */
	unsigned short biPlanes; /* quantidade de planos da imagem	*/
	unsigned short biBitCount;	/* quantidade de bits por pixel */
	unsigned int biCompression; /* compressão, se existit */
	unsigned int biSizeImage; /* tamanho da imagem em bytes */
	unsigned int biXPelsPerMeter; /* número de pixels por metro horizontalmente */
	unsigned int biYPelsPerMeter; /* número de pixels por metro verticalmente */
	unsigned int biClrUsed;	 /* quantidade de cores utilizadas na imagem */
	unsigned int biClrImportant;  /* quantidade de cores importantes */
}BITMAPINFOHEADER;

typedef struct tagBITMAPINFO{  
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD *bmiColors; 
}BITMAPINFO;

typedef struct tagPPMFILEHEADER{
	char type[3];
	int has_comment;
	unsigned int height;
	unsigned int width;
	unsigned int range;
	int offset;
}PPMFILEHEADER;

typedef struct tagBWQUAD{
	unsigned char bwPix, bwReserved;
}BWQUAD;

/**
 * BMP reader/writer
 */
void read_header_BMP(FILE *img, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *info_header);
void write_header_BMP(FILE *img, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header);

/**
 * PPm reader/writer
 */
void read_header_PPM(FILE *img, PPMFILEHEADER *header);
void write_header_PPM(FILE *img, PPMFILEHEADER header);

/*void write_pixels_PGM(BWQUAD **matriz, int height, int width, FILE *img, int offset);*/

/**
 * Reader/writer of pixels in image
 */
void read_pixels(RGBQUAD ***matriz, int height, int width, FILE *img, int offset);
void write_pixels(RGBQUAD **matriz, int height, int width, FILE *img, int offset);

/**
 * Copy an bidimensinal array of RGBQUAD
 * @param matriz The source
 * @param height The height
 * @param width The width
 * @return an copy of the source
 */
RGBQUAD** copy(RGBQUAD **matriz, int height, int width);

void get_comment_ppm(FILE *img, int *comment);

#endif /** __IMAGE_UTILS_H__ */