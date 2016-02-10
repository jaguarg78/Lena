/*
 * Utilities.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: gianni
 */

#include "Utilities.h"

Utilities::Utilities() {
    // TODO Auto-generated constructor stub

}

Utilities::~Utilities() {
    // TODO Auto-generated destructor stub
}

double Utilities::MSE_RGB(const IImageData& rBMPInputImage,
                          const IImageData& rBMPOutputImage) {

    std::cout << "*****MSE_RGB*****" << std::endl;
    std::cout << "Base: " << rBMPInputImage.getFileName() << std::endl;
    std::cout << "Modified: " << rBMPOutputImage.getFileName() << std::endl;
//    std::cout << "[" << rBMPInputImage.getWidth() << "x" << rBMPInputImage.getHeight() << "]"
//              << "[" << rBMPOutputImage.getWidth() << "x" << rBMPOutputImage.getHeight() << "]" << std::endl;
//    for (unsigned int i = 0; i < (rBMPInputImage.getWidth() * rBMPInputImage.getHeight() * sizeof(Pixel)); i++) {
//        std::cout << (int) rBMPInputImage.getData()[i] << " ";
//    }
//    std::cout << std::endl;
//    for (unsigned int i = 0; i < (rBMPOutputImage.getWidth() * rBMPOutputImage.getHeight() * sizeof(Pixel)); i++) {
//        std::cout << (int) rBMPOutputImage.getData()[i] << " ";
//    }
//    std::cout << std::endl;
    if (rBMPInputImage.getWidth() != rBMPOutputImage.getWidth() ||
        rBMPInputImage.getHeight() != rBMPOutputImage.getHeight()) {

        // Throw exception
        throw "Dimensions don't match";
    }

    double dErrorAccumulated = 0.0;
    for (unsigned int uiByteIndex = 0; uiByteIndex < rBMPOutputImage.getDataSize(); uiByteIndex++) {
        std::cout << (double) rBMPInputImage.getConstData()[uiByteIndex] -
                     (double) rBMPOutputImage.getConstData()[uiByteIndex] << " ";
        dErrorAccumulated += std::pow((double) rBMPInputImage.getConstData()[uiByteIndex] -
                                          (double) rBMPOutputImage.getConstData()[uiByteIndex],
                                      2.0);
    }

    std::cout << std::endl << dErrorAccumulated << std::endl;
    return dErrorAccumulated / (rBMPOutputImage.getHeight() * rBMPOutputImage.getWidth() * sizeof(Pixel));
}

double Utilities::PSNR_RGB(const IImageData& rBMPInputImage,
                           const IImageData& rBMPOutputImage) {

    std::cout << "*****PSNR_RGB****" << std::endl;
    std::cout << "Base: " << rBMPInputImage.getFileName() << "[" << rBMPInputImage.getWidth() << "]" << std::endl;
    std::cout << "Modified: " << rBMPOutputImage.getFileName() << "[" << rBMPOutputImage.getWidth() << "]" << std::endl;
    double dMSE_RGB = MSE_RGB(rBMPInputImage, rBMPOutputImage);
    if (dMSE_RGB == 0.) {
        std::cerr << "Images are equal!!!" << std::endl;
        throw "Images are equal!!!";
    }

    std::cout << "MSE : " << dMSE_RGB << std::endl;
    return (20. * std::log10((double) MAX_POSIBLE_PIXEL_VALUE)) - (10. * std::log10(dMSE_RGB));
}

double Utilities::BCR(const IImageData& rBMPInputLogo,
                      const IImageData& rBMPOutputLogo) {

    std::cout << "*****BCR****" << std::endl;
    std::cout << "Base: " << rBMPInputLogo.getFileName() << "[" << rBMPInputLogo.getWidth() << "]" << std::endl;
    std::cout << "Modified: " << rBMPOutputLogo.getFileName() << "[" << rBMPOutputLogo.getWidth() << "]" << std::endl;
    std::cout << "[" << rBMPInputLogo.getWidth() << "x" << rBMPInputLogo.getHeight() << "]"
              << "[" << rBMPOutputLogo.getWidth() << "x" << rBMPOutputLogo.getHeight() << "]" << std::endl;

    if (rBMPInputLogo.getWidth() != rBMPOutputLogo.getWidth() ||
        rBMPInputLogo.getHeight() != rBMPOutputLogo.getHeight()) {

        // Throw exception
        throw "Dimensions don't match";
    }

    unsigned int accumulate = 0;
    unsigned int uiPaddingPerRow = PADDING_ADJUSTMENT - ((rBMPInputLogo.getWidth() / CHAR_BIT) % PADDING_ADJUSTMENT);
    /*
     * uiVWatermarkSeek : Vertical coordinate of binary image logo in bytes
     * uiHWatermarkSeek : Horizontal coordinate of binary image logo in bytes
     */
    for (int uiVWatermarkSeek = 0; uiVWatermarkSeek < rBMPInputLogo.getHeight(); uiVWatermarkSeek++) {
        for (int uiHWatermarkSeek = 0; uiHWatermarkSeek < (rBMPInputLogo.getWidth() / CHAR_BIT); uiHWatermarkSeek++) {
            /* Byte Index into binary image logo Data */
            unsigned int uiByteIndex = (uiVWatermarkSeek * (rBMPInputLogo.getWidth() / CHAR_BIT)) +
                                       uiHWatermarkSeek +
                                       (uiPaddingPerRow * uiVWatermarkSeek);

            char cWatermarkingInputByte;
            char cWatermarkingOutputByte;
            std::memcpy(&cWatermarkingInputByte, &rBMPInputLogo.getConstData()[uiByteIndex], sizeof(char));
            std::memcpy(&cWatermarkingOutputByte, &rBMPOutputLogo.getConstData()[uiByteIndex], sizeof(char));

            /* Bit counter */
            for (unsigned short usWatermarkBit = 0; usWatermarkBit < CHAR_BIT; usWatermarkBit++) {
                // check endianess!!!!!
                unsigned short usInputBinaryBitValue = (unsigned short) (cWatermarkingInputByte & 0x1);
                unsigned short usOutputBinaryBitValue = (unsigned short) (cWatermarkingOutputByte & 0x1);
                cWatermarkingInputByte >>= 1;
                cWatermarkingOutputByte >>= 1;

                accumulate += usInputBinaryBitValue ^ usOutputBinaryBitValue;
            }
        }
    }

    return (((double) accumulate) / (rBMPInputLogo.getHeight() * rBMPInputLogo.getWidth())) * 100;
}
