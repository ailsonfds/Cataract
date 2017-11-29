#ifndef __IMAGE_MANIP_H__
#define __IMAGE_MANIP_H__

#include "image_utils.h"
#include "math_utils.h"

#define THRES_RANGE 200

/**
 * Funções de transformação de imagens
 */
RGBQUAD** bw_transform(RGBQUAD **matriz, int height, int width);
RGBQUAD** gauss_filter(RGBQUAD **matriz, int height, int width);
RGBQUAD** conv_filter(RGBQUAD **pix_image, int img_height, int img_width, double **m_filter, int m_size);
RGBQUAD** sobel_filter(RGBQUAD **matriz, int height, int width);
RGBQUAD** threshold(RGBQUAD **matriz, int height, int width);

/**
 * Algumas funções auxiliares
 */
double pix_mean(RGBQUAD **matriz, int height, int width, char rgb);
double pix_std_deviation(RGBQUAD **matriz, int height, int width, char rgb);
int* circle_detection(RGBQUAD **matriz, int height, int width, int max_r, int min_r);
int *histogram(RGBQUAD **pix_image, int img_height, int img_width);

#endif /*__IMAGE_MANIP_H__*/
