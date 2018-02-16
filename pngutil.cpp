/* Автор Борис Анурин */
#include <stdlib.h>
#include <png.h>
#include "pngutil.h"

int pnguIsPng(FILE *file) {
    png_byte signature[PNGU_SIGNATURE_LEN];
    long file_pos = ftell(file);
    fread(signature, sizeof (png_byte), PNGU_SIGNATURE_LEN, file);
    fseek(file, file_pos, SEEK_SET);
    int is_png = png_sig_cmp(signature, 0, PNGU_SIGNATURE_LEN);
    return is_png == 0;
}

int pnguReadHeader(FILE *file, int *width, int *height, int *byteperpix) {
    int bit_depth, color_type;
    png_infop info_ptr;
    png_structp png_ptr;
    long file_pos;

    file_pos = ftell(file);
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr) return PNGU_ERR_READ_STRUCT;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return PNGU_ERR_INFO_STRUCT;
    }
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 0);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *)width, (png_uint_32 *)height,
                 &bit_depth, &color_type, 0, 0, 0);
    fseek(file, file_pos, SEEK_SET);
    if (color_type == PNG_COLOR_TYPE_RGB) {
        *byteperpix = 3;
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        *byteperpix = 4;
    } else {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return PNGU_ERR_COLOR_TYPE;
    }
    if (bit_depth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return PNGU_ERR_BITDEPTH;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 1;
}

int pnguReadData(FILE *file, unsigned char *pixels) {
    int width, height, bit_depth, color_type, byteperpix, row;
    png_structp png_ptr;
    png_infop info_ptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr) return PNGU_ERR_READ_STRUCT;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return PNGU_ERR_INFO_STRUCT;
    }
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 0);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) & width, (png_uint_32 *) & height,
                 &bit_depth, &color_type, 0, 0, 0);
    if (bit_depth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return PNGU_ERR_BITDEPTH;
    }
    if (color_type == PNG_COLOR_TYPE_RGB) {
        byteperpix = 3;
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        byteperpix = 4;
    } else {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return PNGU_ERR_COLOR_TYPE;
    }
    png_bytep *row_pointers = (png_bytep *)malloc(sizeof (png_bytep) * height);
    for (row = 0; row < height; ++row)
        row_pointers[row] = pixels + row * width * byteperpix;
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(row_pointers);
    return 1;
}

int pnguSavePng(FILE *file, unsigned char *pixels, int width, int height,
                int byteperpix, int complevel) {
    int bitdepth = 8, color_type, row;
    png_bytep *row_pointers;
    png_structp png_ptr;
    png_infop info_ptr;

    if (byteperpix == 3) color_type = PNG_COLOR_TYPE_RGB;
    else if (byteperpix == 4) color_type = PNG_COLOR_TYPE_RGBA;
    else return PNGU_ERR_COLOR_TYPE;

    row_pointers = (png_bytep *)malloc(sizeof (png_bytep) * height);
    for (row = 0; row < height; ++row)
        row_pointers[row] = pixels + row * width * byteperpix;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr) return PNGU_ERR_WRITE_STRUCT;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return PNGU_ERR_INFO_STRUCT;
    }
    png_set_compression_level(png_ptr, complevel);
    png_init_io(png_ptr, file);
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bitdepth, color_type, 0, 0, 0);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row_pointers);
    return 1;
}
