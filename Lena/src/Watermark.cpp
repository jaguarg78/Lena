/*
 * Watermark.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#include "Watermark.h"

//using Eigen::MatrixXd;

Watermark::Watermark(int iWidth,
                     int iHeight,
                     unsigned int iRedundancy,
                     unsigned int iIterations,
                     Type_Coordinates tyCoordinates) : _iWidth(iWidth),
                                                       _iHeight(iHeight),
                                                       _iRedundancy(iRedundancy),
                                                       _iIterations(iIterations),
                                                       _pucInputImageData(NULL),
//                                                       _pucInputLogoData(NULL),
                                                       _pucOutputImageData(NULL),
                                                       _pucOutputLogoData(NULL),
                                                       _tyCoordinates(tyCoordinates) {
	// TODO Auto-generat_ed constructor stub

}

Watermark::~Watermark() throw() {
	if (_pucInputImageData) {
		delete[] _pucInputImageData;
		_pucInputImageData = NULL;
	}

//	if (_pucInputLogoData) {
//		delete[] _pucInputLogoData;
//		_pucInputLogoData = NULL;
//	}

	if (_pucOutputImageData) {
		delete[] _pucOutputImageData;
		_pucOutputImageData = NULL;
	}

	if (_pucOutputLogoData) {
		delete[] _pucOutputLogoData;
		_pucOutputLogoData = NULL;
	}
}

int Watermark::insertLogo(const IImageData& hostImage,
                          const IImageData& logoImage) {
    if (!isEmbeddable(hostImage,
                      logoImage)) {
        // TODO throw exception!!!!!
    }

    _pucInputImageData = hostImage.getData();
    /*
     * The logo image will be traversed in order to embed each pixel into the host image
     * depending on each algorithm
     *
     * uiVLogo : Vertical coordinate of binary image logo
     * uiHLogoByte : Horizontal coordinate of binary image logo in bytes
     */
    for (int uiVLogo = 0;
             uiVLogo < 1;//logoImage.getHeight();
             uiVLogo++) {
        for (unsigned int uiHLogoByte = 0;
                          uiHLogoByte < 1;//logoImage.getRowSizeFixed();
                          uiHLogoByte++) { // H Coordinate of logo image in bytes
            /* Byte Index into binary image logo Data */
            unsigned int uiByteIndex = (uiVLogo * logoImage.getRowSizeFixed()) +
                                       uiHLogoByte;
            /* Byte Info that will be embedded */
            unsigned char ucLogoByte = logoImage.getConstData()[uiByteIndex];
            /* Pixel by byte */
            for (unsigned short usBitPixelIndex = 0;
                                usBitPixelIndex < 1;//CHAR_BIT;
                                usBitPixelIndex++) {

                unsigned char ucPixelBitValue = (ucLogoByte & 0x80) >> 0x07;
                ucLogoByte <<= 1;

                /* usBitRedundancy : Indicates how many times the value will be stored */
                for (unsigned int usBitRedundancy = 0;
                                  usBitRedundancy < _iRedundancy;
                                  usBitRedundancy++) {
                    // These values could be assigned randomly
                    /*
                     * ucPixelBitValue: Value to be embedded.
                     * uiVBlock : Vertical index of the block where bit value will be stored
                     * uiHBlock : Horizontal index of the block where bit value will be stored
                     */
                    InsertionData stInsertionData;
                    stInsertionData.ucPixelBitValue = ucPixelBitValue;
                    stInsertionData.uiVBlock = uiVLogo;
                    stInsertionData.uiHBlock = (((uiHLogoByte * CHAR_BIT) + usBitPixelIndex) *
                                                   _iRedundancy) +
                                               usBitRedundancy;
                    /*
                     * usProcessIteration : Indicates how many times the embedding process
                     * will be repeated on the same block
                     */
                    for (unsigned int usProcessIteration = 0;
                                      usProcessIteration < _iIterations;
                                      usProcessIteration++) {

                        insertProcess(hostImage,
                                      stInsertionData);
                    } // usProcessIteration
                } // usBitRedundancy
            } // usWatermarkBit
        } // uiHWatermarkSeek
    } // uiVWatermarkSeek

	return 0;
}

void Watermark::insertProcess(const IImageData& hostImage,
                              const InsertionData& stInsertionData) {

    std::cout << "*************** [" << stInsertionData.uiVBlock << " "
                                     << stInsertionData.uiHBlock << "] -> "
                                     << (int) stInsertionData.ucPixelBitValue << std::endl;
    /* Create blocks where bits will be embedded */
    Eigen::MatrixXd mBlock = getBlockData(hostImage,
                                          stInsertionData);

    std::cout << mBlock << std::endl;
    std::cout << std::endl;
    /* Calculate embedded block */
    Eigen::MatrixXd mWMBlock = processBlock(mBlock, stInsertionData.ucPixelBitValue);
    std::cout << mWMBlock << std::endl;
//        setBlockData(uiVBlock,
//                     uiHBlock,
//                     hostImage,
//                     mWMBlock);
}

Eigen::MatrixXd Watermark::getBlockData(const IImageData& hostImage,
                                        const InsertionData& stInsertionData) {

    Eigen::MatrixXd mBlock(_iWidth, _iHeight);

    for (int iBlockRow = 0; iBlockRow < _iHeight; iBlockRow++) {
        for (int iBlockCol = 0; iBlockCol < _iWidth; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((stInsertionData.uiVBlock * _iHeight) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((stInsertionData.uiHBlock * _iWidth) + iBlockCol))) * sizeof(Pixel));

            Pixel pixel;
            std::memcpy((char *) &pixel,
                        hostImage.getConstData() + uiDataOffset,
                        sizeof(Pixel));
            /* Validate if it is a Gray Scale pixel */
            if ((pixel.byteRed == pixel.byteGreen) &&
                (pixel.byteRed == pixel.byteBlue)) {

                mBlock(iBlockRow, iBlockCol) = (double) ((int) pixel.byteRed & 0xFF);
            } else {
                std::ostringstream oss;
                oss << "Block [" << stInsertionData.uiVBlock << "," << stInsertionData.uiHBlock << "]"
                          << "[" << iBlockRow << ", " << iBlockCol << "]"
                          << "[" << pixel.byteRed << "," << pixel.byteGreen << "," << pixel.byteBlue << "]";

                std::cout << oss.str() << std::endl;
                throw oss.str();
                // TODO Exception
            }
        } // iBlockCol
    } // iBlockRow

    return mBlock;
}

void Watermark::setBlockData(IImageData& hostImage,
                             const InsertionData& stInsertionData,
                             const Eigen::MatrixXd& mWMBlock) {

    for (int iBlockRow = 0; iBlockRow < _iHeight; iBlockRow++) {
        for (int iBlockCol = 0; iBlockCol < _iWidth; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((stInsertionData.uiVBlock * _iHeight) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((stInsertionData.uiHBlock * _iWidth) + iBlockCol))) * sizeof(Pixel));

            unsigned char byGrayLevel = round((double) mWMBlock(iBlockRow, iBlockCol));
            Pixel pixel;
            pixel.byteRed   = byGrayLevel;
            pixel.byteGreen = byGrayLevel;
            pixel.byteBlue  = byGrayLevel;
            std::memcpy(hostImage.getData() + uiDataOffset,
                        (char *) &pixel,
                        sizeof(Pixel));
        } // iBlockCol
    } // iBlockRow
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
