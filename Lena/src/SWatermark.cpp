/*
 * SWatermark.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: gianni
 */

#include "SWatermark.h"

SWatermark::SWatermark(int iWidth,
                       int iHeight,
                       int iDelta) : Watermark(iWidth, iHeight),
                                     _iDelta(iDelta){
    // TODO Auto-generated constructor stub
}

SWatermark::~SWatermark() throw() {
    // TODO Auto-generated destructor stub
}

bool SWatermark::isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage) {
//    std::cout << "Dimensions" << std::endl;
//    std::cout << logoImage.getWidth() << std::endl;
//    std::cout << hostImage.getRowSizeFixed() << std::endl;
//    std::cout << logoImage.getHeight() << std::endl;
//    std::cout << hostImage.getHeight() << std::endl;
    return (unsigned int) logoImage.getWidth() * REDUDANCY_PER_BINARY_PIXEL * _iWidth <= hostImage.getRowSizeFixed() && // Horizontal validation
           logoImage.getHeight() * _iHeight <= hostImage.getHeight(); // Vertical validation
}

void SWatermark::insertProcess(const IImageData& hostImage,
                               unsigned char ucPixelBitValue, // Value to be embedded
                               unsigned int uiVLogo, // V Coordinate of logo image
                               unsigned int uiHLogoByte, // H Coordinate of logo image in bytes
                               unsigned short usBitPixelIndex) { // Index into byte of value
    /* usBitRedundancy : Indicates how many times the value will be stored */
    for (unsigned short usBitRedundancy = 0;
                        usBitRedundancy < REDUDANCY_PER_BINARY_PIXEL;
                        usBitRedundancy++) {
        // These values could be assigned randomly
        /*
         * uiVBlock : Vertical coordinate of the block where bit value will be stored
         * uiHBlock : Horizontal coordinate of the block where bit value will be stored
         */
        unsigned int uiVBlock = uiVLogo;
        unsigned int uiHBlock = (((uiHLogoByte * CHAR_BIT) + usBitPixelIndex) *
                                    REDUDANCY_PER_BINARY_PIXEL) + // This value could be a parameter
                                usBitRedundancy;

        /* Create blocks where bits will be embedded */
        Eigen::MatrixXd mBlock = getBlockData(uiVBlock,
                                              uiHBlock,
                                              hostImage);

        std::cout << "*************** [" << uiVBlock << " " << uiHBlock << "] -> " << (int) ucPixelBitValue << std::endl;
        std::cout << mBlock << std::endl;
        /* Calculate embedded block */
        Eigen::MatrixXd mWMBlock = processBlock(mBlock, ucPixelBitValue);
        std::cout << "*************** [" << uiVBlock << " " << uiHBlock << "] -> " << (int) ucPixelBitValue << std::endl;
        std::cout << mWMBlock << std::endl;
//        setBlockData(uiVBlock,
//                     uiHBlock,
//                     hostImage,
//                     mWMBlock);
    } // usBitRedundancy
}

Eigen::MatrixXd SWatermark::getBlockData(unsigned int uiVBlock,
                                         unsigned int uiHBlock,
                                         const IImageData& hostImage) {

    Eigen::MatrixXd mBlock(_iWidth, _iHeight);

    for (int iBlockRow = 0; iBlockRow < _iHeight; iBlockRow++) {
        for (int iBlockCol = 0; iBlockCol < _iWidth; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((uiVBlock * _iHeight) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((uiHBlock * _iWidth) + iBlockCol))) * sizeof(Pixel));

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
                oss << "Block [" << uiVBlock << "," << uiHBlock << "]"
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

void SWatermark::setBlockData(unsigned int uiVBlock,
                              unsigned int uiHBlock,
                              IImageData& hostImage,
                              const Eigen::MatrixXd& mWMBlock) {

    for (int iBlockRow = 0; iBlockRow < _iHeight; iBlockRow++) {
        for (int iBlockCol = 0; iBlockCol < _iWidth; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((uiVBlock * _iHeight) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((uiHBlock * _iWidth) + iBlockCol))) * sizeof(Pixel));

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

Eigen::MatrixXd SWatermark::processBlock(const Eigen::MatrixXd& mBlock,
                                         unsigned char ucPixelBitValue) {
    /* Processing data using SVD */
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(mBlock, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd vSingularValues = svd.singularValues();

    // Paper 00 - 35
    // Step 3: s3 = s2
    vSingularValues[2] = vSingularValues[1];
    // Step 4: s2 = s2 + (delta * bit value)
    vSingularValues[1] = vSingularValues[1] + (_iDelta * ucPixelBitValue);
    // if s1 < new s2 then it will be equal to the new s2
    if (vSingularValues[0] < vSingularValues[1]) {
        vSingularValues[0] = vSingularValues[1];
    }

    /* Calculate embedded block */
    return svd.matrixU() * vSingularValues.asDiagonal() * svd.matrixV().transpose();
}
