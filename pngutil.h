/* Автор Борис Анурин */
/*
   Библиотека, для загрузки файлов формата PNG.
   Поддерживает форматы пикселей RGB и RGBA, по 8бит на цвет.
   pnguIsPng() and pnguReadHeader() не сдвигают файловый указатель.
*/

#ifndef REALMANTEAM_ENGINE_PNGUTIL_H_
#define REALMANTEAM_ENGINE_PNGUTIL_H_

#include <stdio.h>

#define PNGU_SIGNATURE_LEN		8

#define PNGU_ERR_COLOR_TYPE 	2
#define PNGU_ERR_WRITE_STRUCT	3
#define PNGU_ERR_INFO_STRUCT	4
#define PNGU_ERR_READ_STRUCT	5
#define PNGU_ERR_BITDEPTH 		6

#define PNGU_COMPLEVEL_MAX		9
#define PNGU_COMPLEVEL_MIN		0

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* read signaure and return 1(true) or 0(false)*/
int pnguIsPng(FILE *file);
int pnguReadHeader(FILE *file, int *width, int *height, int *byteperpix);
/* pixels must have enough size */
int pnguReadData(FILE *file, unsigned char *pixels);
int pnguSavePng(FILE *file, unsigned char *pixels, int width, int height,
                int byteperpix, int complevel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* REALMANTEAM_ENGINE_PNGUTIL_H_ */