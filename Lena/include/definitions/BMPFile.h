/*
 * BMPFile.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef INCLUDE_BMPFILE_H_
#define INCLUDE_BMPFILE_H_

#include "Global.h"

#pragma pack(2)

typedef struct {
    unsigned short  bfType;             //  2 bytes
    unsigned int    bfSize;             //  4 bytes
    unsigned short  bfReserved1;        //  2 bytes
    unsigned short  bfReserved2;        //  2 bytes
    unsigned int    bfOffBits;          //  4 bytes
} BitMapFileHeader;                     // 14 bytes

typedef struct {
    unsigned int    biSize;             //  4 bytes
    unsigned int    Width;              //  4 bytes
    unsigned int    Height;             //  4 bytes
    unsigned short  biPlanes;           //  2 bytes
    unsigned short  biBitCount;         //  2 bytes
    unsigned int    biCompresssion;     //  4 bytes
    unsigned int    biSizeImage;        //  4 bytes
    unsigned int    biXPelsPerMeter;    //  4 bytes
    unsigned int    biYPelsPerMeter;    //  4 bytes
    unsigned int    biClrUsed;          //  4 bytes
    unsigned int    biClrImportant;     //  4 bytes
} BitMapInfoHeader;                     // 40 bytes

#pragma pack()

#endif /* INCLUDE_BMPFILE_H_ */
