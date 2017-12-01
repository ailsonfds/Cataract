#ifndef __CATARACT_H__
#define __CATARACT_H__

#include "image_manip.h"

RGBQUAD** apply_necessary_filters(RGBQUAD** img, int height, int width, char* output_name);
int* circle_detection(RGBQUAD** img, int height, int width, int max_r, int min_r);
void make_votation(RGBQUAD** img, int height, int width, int max_r, int min_r, int* matriz_a, int *x_center, int *y_center, int *r_center);
RGBQUAD** segmented_image(RGBQUAD** img, int height, int width, int r_center, int x_center, int y_center);
void make_diagnoses(PPMFILEHEADER ppm_reader, RGBQUAD** img, char* output_name);

double ** matriz_filtro(int ordem, double desvio);
double ** matriz_filtro_gaussiano(double desvio);

#endif /* __CATARACT_H__ */