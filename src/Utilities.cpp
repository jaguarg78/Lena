/*
 * Utilities.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: gianni
 */

#include "Utilities.h"

namespace Utilities {

    double MSE_RGB(const IImageData& rBMPInputImage,
                   const IImageData& rBMPOutputImage) {

        //DEBUG
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

        /* Check dimensions match */
        if (rBMPInputImage.getWidth() != rBMPOutputImage.getWidth() ||
            rBMPInputImage.getHeight() != rBMPOutputImage.getHeight()) {

            // Throw exception
            throw "Dimensions don't match";
        }

        double dErrorAccumulated = 0.0;
        for (unsigned int uiByteIndex = 0; uiByteIndex < rBMPOutputImage.getDataSize(); uiByteIndex++) {
            // DEBUG
//            std::cout << (double) rBMPInputImage.getConstData()[uiByteIndex] -
//                         (double) rBMPOutputImage.getConstData()[uiByteIndex] << " ";
            dErrorAccumulated += std::pow((double) rBMPInputImage.getConstData()[uiByteIndex] -
                                              (double) rBMPOutputImage.getConstData()[uiByteIndex],
                                          2.0);
        }

        std::cout << std::endl << dErrorAccumulated << std::endl;
        return dErrorAccumulated / (rBMPOutputImage.getHeight() * rBMPOutputImage.getWidth() * sizeof(Pixel));
    }

    double PSNR_RGB(const IImageData& rBMPInputImage,
                    const IImageData& rBMPOutputImage) {

        // DEBUG
        std::cout << "*****PSNR_RGB****" << std::endl;
        std::cout << "Base: " << rBMPInputImage.getFileName() << "[" << rBMPInputImage.getWidth() << "]" << std::endl;
        std::cout << "Modified: " << rBMPOutputImage.getFileName() << "[" << rBMPOutputImage.getWidth() << "]" << std::endl;

        double dMSE_RGB = MSE_RGB(rBMPInputImage, rBMPOutputImage);
        if (dMSE_RGB == 0.) {
            std::cerr << "Images are equal!!!" << std::endl;
            throw "Images are equal!!!";
        }

        // DEBUG
        std::cout << "MSE : " << dMSE_RGB << std::endl;
        return (20. * std::log10((double) PIXEL_CHANNEL_MAX)) - (10. * std::log10(dMSE_RGB));
    }

    double BCR(const IImageData& rBMPInputLogo,
               const IImageData& rBMPOutputLogo) {

        // DEBUG
        std::cout << std::endl << "*****BCR****" << std::endl;
        std::cout << "Base: " << rBMPInputLogo.getFileName() << "[" << rBMPInputLogo.getWidth() << "]" << std::endl;
        std::cout << "Modified: " << rBMPOutputLogo.getFileName() << "[" << rBMPOutputLogo.getWidth() << "]" << std::endl;
        std::cout << "[" << rBMPInputLogo.getWidth() << "x" << rBMPInputLogo.getHeight() << "]"
                  << "[" << rBMPOutputLogo.getWidth() << "x" << rBMPOutputLogo.getHeight() << "]" << std::endl;

        /* Check diemnsions match */
        if (rBMPInputLogo.getWidth() != rBMPOutputLogo.getWidth() ||
            rBMPInputLogo.getHeight() != rBMPOutputLogo.getHeight()) {

            // TODO Throw exception
            throw "Dimensions don't match";
        }

        std::vector<unsigned char> inputData(rBMPInputLogo.getData(),
                                             rBMPInputLogo.getData() + rBMPInputLogo.getDataSize());
        std::vector<unsigned char> outputData(rBMPOutputLogo.getData(),
                                              rBMPOutputLogo.getData() + rBMPOutputLogo.getDataSize());

        unsigned int uiDimension = rBMPInputLogo.getWidth();
        unsigned int uiPaddingBytes = rBMPInputLogo.getRowSizeFixed();
        return BCR(inputData,
                   outputData,
                   &uiDimension,
                   &uiPaddingBytes);
    }

    double BCR(const std::vector<unsigned char>& inputData,
               const std::vector<unsigned char>& outputData,
               unsigned int *puiDimension,
               unsigned int *puiPaddingBytes) {


        if (inputData.size() != outputData.size()) {
            // TODO throw related exception
            throw "Size Data don't match";
        }

        if (puiDimension && !puiPaddingBytes) {
            // TODO throw related exception
            throw "Parameter is missing";
        }

        // DEBUG
        std::cout << "Data Size: " << inputData.size() << std::endl;

        unsigned int uiAccumulated = 0;
        unsigned int uiColumnIndex = 0;

        for (unsigned int uiDataByteIndex = 0; uiDataByteIndex < inputData.size(); uiDataByteIndex++) {
            unsigned char ucInputDataByte = inputData.data()[uiDataByteIndex];
            unsigned char ucOutputDataByte = outputData.data()[uiDataByteIndex];

            for (unsigned int uiBitIndex = 0; uiBitIndex < CHAR_BIT; uiBitIndex++) {

                uiAccumulated += ((ucInputDataByte & 0x80) >> 0x07) ^ ((ucOutputDataByte & 0x80) >> 0x07);

                // DEBUG
                std::cout << "[" << (uiDataByteIndex * CHAR_BIT) + uiBitIndex << "]"
                                 << ((ucInputDataByte & 0x80) >> 0x07) << ", "
                                 << ((ucOutputDataByte & 0x80) >> 0x07) << " ->"
                                 << uiAccumulated << std::endl;

                ucInputDataByte <<= 0x01;
                ucOutputDataByte <<= 0x01;

                if (puiDimension) {
                    uiColumnIndex++;
                    if (uiColumnIndex == (*puiDimension)) {
                        uiColumnIndex = 0;

                        // DEBUG
                        std::cout  << "[" << (*puiPaddingBytes) << "] "
                                          << (uiDataByteIndex % (*puiPaddingBytes)) << std::endl;

                        std::cout << "Prev: " <<  uiDataByteIndex << std::endl;
                        uiDataByteIndex += ((*puiPaddingBytes) - (uiDataByteIndex % (*puiPaddingBytes))) - 1;

                        std::cout << "Post: " <<  uiDataByteIndex << std::endl;
                        break;
                    }
                }
            }
        }

        if (puiDimension) {
            // DEBUG
            std::cout << "Acc : " << uiAccumulated << std::endl;
            std::cout << "Dim : " << (*puiDimension) << std::endl;
            return (((double) uiAccumulated) / ((*puiDimension) * (*puiDimension))) * 100;
        }

        return (((double) uiAccumulated) / inputData.size()) * 100;
    }

    void toGrayScale(IImageData& rBMPRawImage,
                     GRAY_CALC eGrayCalc) {
        // Warning!!!! Validate Depth. It only works with RGB888
        for (int iHeight = 0; iHeight < rBMPRawImage.getHeight(); iHeight++) {
            for (int iWidth = 0; iWidth < rBMPRawImage.getWidth(); iWidth++) {
                unsigned int uiOffset = (iHeight * rBMPRawImage.getRowSizeFixed()) + (iWidth * sizeof(Pixel));

                Pixel pixel;
                std::memcpy(&pixel,
                            rBMPRawImage.getData() + uiOffset,
                            sizeof(Pixel));
                unsigned char ucGrayLevel = 0x00;
                switch (eGrayCalc) {
                case GRAY_MEAN:
                    ucGrayLevel = (unsigned char) round((pixel.byteRed + pixel.byteGreen + pixel.byteBlue) / 3);
                    break;
                case GRAY_LIGHTNESS:
                    ucGrayLevel = (unsigned char) round((std::min(pixel.byteRed,
                                                             std::min(pixel.byteGreen,
                                                                      pixel.byteBlue)) +
                                                         std::max(pixel.byteRed,
                                                             std::max(pixel.byteGreen,
                                                                      pixel.byteBlue))) / 2);
                    break;
                case GRAY_LUMINOSITY:
                    ucGrayLevel = (unsigned char) round(0.21 * pixel.byteRed +
                                                        0.72 * pixel.byteGreen +
                                                        0.07 * pixel.byteBlue);
                    break;
                default:
                    throw;
                }

                pixel.byteRed = ucGrayLevel;
                pixel.byteGreen = ucGrayLevel;
                pixel.byteBlue = ucGrayLevel;

                std::memcpy(rBMPRawImage.getData() + uiOffset,
                            &pixel,
                            sizeof(Pixel));
            }
        }
    }

    SVD getSVD(Eigen::MatrixXd matA) {

        int iSize = matA.rows();
        int iLeadWS = -1;
        int iInfo;
        double dOptWS;
        Eigen::MatrixXd mATemp = matA;
#ifdef _DEBUG
        std::cout << "Matrix A" << std::endl;
		std::cout << matA << std::endl;
		std::cout << "Matrix A copy" << std::endl;
		std::cout << mATemp << std::endl;
#endif
		SVD svd;
		svd.matU.resize(iSize, iSize);
		svd.matS.resize(iSize, iSize);
		svd.matV.resize(iSize, iSize);

		Eigen::VectorXd vSTemp(iSize);
		Eigen::MatrixXd mVTemp(iSize, iSize);
        /* Getting Optimal Workspace size */
        ::dgesvd_(const_cast<char *>(std::string("All").c_str()),
                  const_cast<char *>(std::string("All").c_str()),
                  &iSize,
                  &iSize,
                  mATemp.data(),
                  &iSize,
                  vSTemp.data(),
                  svd.matU.data(),
                  &iSize,
                  mVTemp.data(),
                  &iSize,
                  &dOptWS,
                  &iLeadWS,
                  &iInfo);
        iLeadWS = (int) dOptWS;

        double *pdWS = new double[iLeadWS * sizeof(double)];
        ::dgesvd_(const_cast<char *>(std::string("All").c_str()),
                  const_cast<char *>(std::string("All").c_str()),
                  &iSize,
                  &iSize,
                  mATemp.data(),
                  &iSize,
                  vSTemp.data(),
                  svd.matU.data(),
                  &iSize,
                  mVTemp.data(),
                  &iSize,
                  pdWS,
                  &iLeadWS,
                  &iInfo);
        svd.matV = mVTemp.transpose();
        svd.matS = vSTemp.asDiagonal();
#if _DEBUG
        std::cout << "Matrix U" << std::endl;
		std::cout << svd.matU << std::endl;
		std::cout << "Vector S" << std::endl;
		std::cout << svd.matS << std::endl;
		std::cout << "Matrix V" << std::endl;
		std::cout << svd.matV << std::endl;
		std::cout << "Matrix A Result" << std::endl;
		std::cout << svd.matU * svd.matS * svd.matV.transpose() << std::endl;
#endif
        return svd;
    }

    void normalizeValues(Eigen::MatrixXd& mBlock) {
        for (int iRow = 0; iRow < mBlock.rows(); iRow++) {
            for (int iCol = 0; iCol < mBlock.cols(); iCol++) {
                mBlock(iRow, iCol) = (mBlock(iRow, iCol) > 255. ?
                                          255. :
                                          mBlock(iRow, iCol) < 0. ?
                                                  0. :
                                                  round((double) mBlock(iRow, iCol)));
            }
        }
    }

    /* Debug functions */
    namespace Debug {
        const std::string getValuesFromVector(const std::vector<unsigned char>& vecInput,
                                              unsigned int uiOffset,
                                              unsigned int uiWidth) {

            // DEBUG
//            std::cout << "Size   :" << vecInput.size() << std::endl;
//            std::cout << "Offset :" << uiOffset << std::endl;
//            std::cout << "Width  :" << uiWidth << std::endl;

            std::ostringstream oss;
            for (unsigned int i = 0; i < vecInput.size(); i++) {
                if ((uiOffset + i) % uiWidth == 0) {
                    oss << std::endl;
                }

                oss << (int) vecInput[i];

                if (i != (vecInput.size() - 1)) {
                    oss << " ";
                }
            }
            oss << "|";

            return oss.str();
        }
    }
}
