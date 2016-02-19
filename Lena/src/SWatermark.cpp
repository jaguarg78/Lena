/*
 * SWatermark.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: gianni
 */

#include "SWatermark.h"

SWatermark::SWatermark(int iWidth,
                       int iHeight,
                       int iDelta,
                       unsigned int iRedundancy,
                       unsigned int iIterations,
                       Type_Coordinates tyCoordinates) : Watermark(iWidth,
                                                                   iHeight,
                                                                   iRedundancy,
                                                                   iIterations,
                                                                   tyCoordinates),
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
