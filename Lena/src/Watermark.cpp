/*
 * Watermark.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#include "Watermark.h"

//using Eigen::MatrixXd;

Watermark::Watermark(int iWidth,
                     int iHeight) : _iWidth(iWidth),
                                    _iHeight(iHeight)/*,
                                    _pucInputImageData(NULL),
                                    _pucInputLogoData(NULL),
                                    _pucOutputImageData(NULL),
                                    _pucOutputLogoData(NULL)*/ {
	// TODO Auto-generat_ed constructor stub

}

Watermark::~Watermark() throw() {
//	if (_pucInputImageData) {
//		delete[] _pucInputImageData;
//		_pucInputImageData = NULL;
//	}
//
//	if (_pucInputLogoData) {
//		delete[] _pucInputLogoData;
//		_pucInputLogoData = NULL;
//	}
//
//	if (_pucOutputImageData) {
//		delete[] _pucOutputImageData;
//		_pucOutputImageData = NULL;
//	}
//
//	if (_pucOutputLogoData) {
//		delete[] _pucOutputLogoData;
//		_pucOutputLogoData = NULL;
//	}
}

int Watermark::insertLogo(const IImageData& hostImage,
                          const IImageData& logoImage) {

    if (!isEmbeddable(hostImage,
                      logoImage)) {
        // TODO throw exception!!!!!
    }

    /*
     * The logo image will be traversed in order to embed each pixel into the host image
     * depending on each algorithm
     */
    /*
     * uiVLogo : Vertical coordinate of binary image logo
     * uiHLogoByte : Horizontal coordinate of binary image logo in bytes
     */
    for (int uiVLogo = 0;
             uiVLogo < logoImage.getHeight();
             uiVLogo++) {
        for (unsigned int uiHLogoByte = 0;
                          uiHLogoByte < logoImage.getRowSizeFixed();
                          uiHLogoByte++) {
            /* Byte Index into binary image logo Data */
            unsigned int uiByteIndex = (uiVLogo * logoImage.getRowSizeFixed()) +
                                       uiHLogoByte;
            /* Byte Info that will be embedded */
            unsigned char ucLogoByte = logoImage.getConstData()[uiByteIndex];
            /* Pixel by byte */
            for (unsigned short usBitPixelIndex = 0;
                                usBitPixelIndex < CHAR_BIT;
                                usBitPixelIndex++) {

                unsigned char ucPixelBitValue = (ucLogoByte & 0x80) >> 0x07;
                ucLogoByte <<= 1;

                insertProcess(hostImage,
                              ucPixelBitValue,
                              uiVLogo,
                              uiHLogoByte,
                              usBitPixelIndex);

            } // usWatermarkBit
        } // uiHWatermarkSeek
    } // uiVWatermarkSeek

	return 0;
}

int Watermark::extractLogo() {
	return 0;
}

void Watermark::showProgress() {

}

//void Watermark::setWidth(int iWidth) {
//	_iWidth = iWidth;
//}
//
//void Watermark::setHeight(int iHeight) {
//	_iHeight = iHeight;
//}
//
//void Watermark::setInputImageData(unsigned char *pucInputImageData) {
//	_pucInputImageData = pucInputImageData;
//}
//
//void Watermark::setInputLogoData(unsigned char *pucInputLogoData) {
//	_pucInputLogoData = pucInputLogoData;
//}
//
//void Watermark::setOutputImageData(unsigned char *pucOutputImageData) {
//	_pucOutputImageData = pucOutputImageData;
//}
//
//void Watermark::setOutputLogoData(unsigned char *pucOutputLogoData) {
//	_pucOutputLogoData = pucOutputLogoData;
//}
