/*
 * Global.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef INCLUDE_GLOBAL_H_
#define INCLUDE_GLOBAL_H_

#include <iostream>
#include <climits>

enum {
	TYPE_I,
	TYPE_II,
	TYPE_III,
	TYPE_IV,
	TYPE_NUMS
};

typedef struct {
    unsigned char   byteBlue;           // 1 byte
    unsigned char   byteGreen;          // 1 byte
    unsigned char   byteRed;            // 1 byte
} Pixel;

typedef struct {
    Pixel           pixel;              // 3 bytes
    unsigned char   byteUnused;         // 1 byte
} PixelTable;

typedef struct {
    double  *pdData;
    int      iRows;
    int      iColumns;
    int      iMatrixOrder;
} DMatrix;

#define BIT_COUNT_BINARY_IMAGE       1
#define BIT_COUNT_8_BIT_IMAGE        8
#define BIT_COUNT_RGB_IMAGE          sizeof(Pixel) * CHAR_BIT

#define MAX_POSIBLE_PIXEL_VALUE      255

#define PADDING_ADJUSTMENT           4

#endif /* INCLUDE_GLOBAL_H_ */
