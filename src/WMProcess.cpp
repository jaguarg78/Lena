/*
 * WMProcess.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: gianni
 */

#include "WMProcess.h"

namespace SProcess {

    /* Start WMProcess_17 Class */
    WMProcess_17::WMProcess_17(int iDelta,
                               Type_17 type,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : Watermark(uiDimension,
                                                                           uiRedundancy,
                                                                           uiIterations,
                                                                           DATA_SIZE_PER_BLOCK_DEFAULT,
                                                                           tyCoordinates),
                                                                 _iDelta (iDelta),
                                                                 _type(type) {

        // DEBUG
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_17" << std::endl;
        std::cout << "************************************" << std::endl;

    }

    WMProcess_17::~WMProcess_17() throw() {

    }

    bool WMProcess_17::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_17::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_17::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                               const std::vector<unsigned char>& vecData) {

        // DEBUG
//        std::cout << "[D: " << _iDelta << "]" << std::endl;

        // Only if s1 > s2
        Eigen::VectorXd vecSingValues = rSVD.singularValues();

        // DEBUG
//        std::cout << "Singular Values Pre" << std::endl;
//        std::cout << (Eigen::MatrixXd) vecSingValues.asDiagonal() << std::endl;

        // Step 4: |s2 - s3|
        float fDifference = std::abs((float) (vecSingValues(1) - vecSingValues(2)));

        // s4' assignation
        if (_type == TYPE_17_1) {
            // s4' according to Chang, Lin, Hu [1-17]
            vecSingValues(3) = (vecSingValues(2) > fDifference) ? fDifference : -fDifference;
        } else if (_type == TYPE_17_2) {
            // s4' according to Chanu, Singh, Tuithung [0-01]
            vecSingValues(3) = (vecSingValues(2) > fDifference) ? fDifference : 0;
        } else {
            throw;
        }

        /* It has only one value */
        // Step 5: s2' = s2 + (delta * bitValue)
        vecSingValues(1) =  vecSingValues(1) + (_iDelta * vecData[0]);

        // DEBUG
        std::cout << "Singular Values Post" << std::endl;
        std::cout << "Data to make the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) (-1 * rSVD.matrixU()) << std::endl << std::endl
                  << (Eigen::MatrixXd) vecSingValues.asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) (-1 * rSVD.matrixV()) << std::endl << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = (-1 * rSVD.matrixU()) * vecSingValues.asDiagonal() * (-1 * rSVD.matrixV().transpose());
//        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
//        std::cout << "Normalized Matrix to be returned" << std::endl;
//        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_17::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {

        // DEBUG
            std::cout << "Data to find the embedded value" << std::endl;
            std::cout << rSVD.matrixU() << std::endl << std::endl
                      << rSVD.singularValues() << std::endl << std::endl
                      << rSVD.matrixV() << std::endl << std::endl;
//            std::cout << "The analized matrix" << std::endl;
//            std::cout << (rSVD.matrixU() * rSVD.singularValues().asDiagonal() * rSVD.matrixV().transpose());
//            std::cout << std::endl << std::endl;

//        std::vector<unsigned char>  vecExtractedValues(_uiDataPerBlock);
//        vecExtractedValues[0] = (rSVD.singularValues()(1) - rSVD.singularValues()(2) > (_iDelta / 2)) ? 0x01 : 0x00;
//        return vecExtractedValues;

        stBlockData.vecLogoDataBlock[0] += (rSVD.singularValues()(1) - rSVD.singularValues()(2) > (_iDelta / 2)) ? 0x01 : 0x00;
    }
    /* End WMProcess_17 Class */
//#if 0
    /* Start WMProcess_35 Class */
    WMProcess_35::WMProcess_35(int iDelta,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : Watermark(uiDimension,
                                                                           uiRedundancy,
                                                                           uiIterations,
                                                                           DATA_SIZE_PER_BLOCK_DEFAULT,
                                                                           tyCoordinates),
                                                                 _iDelta (iDelta) {

        // DEBUG
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_35" << std::endl;
        std::cout << "************************************" << std::endl;
    }

    WMProcess_35::~WMProcess_35() throw() {

    }

    bool WMProcess_35::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_35::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_35::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                               const std::vector<unsigned char>& vecData) {

        // DEBUG
        std::cout << "[D: " << _iDelta << "]" << std::endl;

        // TODO Warning about block selection!!!!
        Eigen::VectorXd vecSingValues = rSVD.singularValues();

        // DEBUG
        std::cout << "Singular Values Pre" << std::endl;
        std::cout << (Eigen::MatrixXd) vecSingValues.asDiagonal() << std::endl;

//            std::cout << "Singular Values Pre" << std::endl;
//            std::cout << vecSingValues << std::endl;
        // Step 3: s3' = s2
        vecSingValues(2) = vecSingValues(1);

        // Step 5: s2' = s2 + (delta * bitValue)
        vecSingValues(1) =  vecSingValues(1) + (_iDelta * vecData[0]);

        // Step 6: if s1 < s2' -> s1' = s2 + (delta * bitValue)
        if (vecSingValues(0) < vecSingValues(1)) {
            vecSingValues(0) =  vecSingValues(1);
        }

        // DEBUG
        std::cout << "Singular Values Post" << std::endl;
        std::cout << "Data to made the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) (-1 * rSVD.matrixU()) << std::endl << std::endl
                  << (Eigen::MatrixXd) vecSingValues.asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) (-1 * rSVD.matrixV()) << std::endl << std::endl;
        std::cout << "Matrix to be returned" << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = (-1 * rSVD.matrixU()) * vecSingValues.asDiagonal() * (-1 * rSVD.matrixV().transpose());
        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_35::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {

        // DEBUG
//            std::cout << "Data to find the embedded value" << std::endl;
//            std::cout << rSVD.matrixU() << std::endl << std::endl
//                      << rSVD.singularValues() << std::endl << std::endl
//                      << rSVD.matrixV() << std::endl << std::endl;
//            std::cout << "The analized matrix" << std::endl;
//            std::cout << (rSVD.matrixU() * rSVD.singularValues().asDiagonal() * rSVD.matrixV().transpose());
//            std::cout << std::endl << std::endl;

        stBlockData.vecLogoDataBlock[0] += (rSVD.singularValues()(1) - rSVD.singularValues()(2) > (_iDelta / 2)) ? 0x01 : 0x00;
    }
//#endif
    /* End WMProcess_35 Class */
//#if 0
    /* Start WMProcess_74 Class */
    WMProcess_74::WMProcess_74(float fModule,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : Watermark(uiDimension,
                                                                           uiRedundancy,
                                                                           uiIterations,
                                                                           DATA_SIZE_PER_BLOCK_DEFAULT,
                                                                           tyCoordinates),
                                                                 _fQModule(fModule) {

        // DEBUG
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_74" << std::endl;
        std::cout << "************************************" << std::endl;
    }

    WMProcess_74::~WMProcess_74() throw() {

    }

    bool WMProcess_74::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_74::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_74::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                               const std::vector<unsigned char>& vecData) {

        // Warning about block selection!!!!
        Eigen::VectorXd vecSingValues = rSVD.singularValues();
//            std::cout << "Singular Values Pre" << std::endl;
//            std::cout << vecSingValues << std::endl;
        // Step 3: z_i,j = D(1,1) mod Q
        float fZQuantizedValue = std::fmod((float) vecSingValues(1), _fQModule);

        // Step 4, 5: D(1,1) =
        // In case of 0
        if (vecData[0] == 0x00) {
            if (fZQuantizedValue < (3 * _fQModule / 4)) {
                vecSingValues(1) = vecSingValues(1) + (_fQModule / 4) - fZQuantizedValue;
            } else {
                vecSingValues(1) = vecSingValues(1) + (5 * _fQModule / 4) - fZQuantizedValue;
            }
        // In case of 1
        } else {
            if (fZQuantizedValue < (_fQModule / 4)) {
                vecSingValues(1) = vecSingValues(1) - (_fQModule / 4) + fZQuantizedValue;
            } else {
                vecSingValues(1) = vecSingValues(1) + (3 * _fQModule / 4) - fZQuantizedValue;
            }
        }


        // DEBUG
        std::cout << "Data to made the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) (-1 * rSVD.matrixU()) << std::endl << std::endl
                  << (Eigen::MatrixXd) vecSingValues.asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) (-1 * rSVD.matrixV()) << std::endl << std::endl;
        std::cout << "Matrix to be returned" << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = (-1 * rSVD.matrixU()) * vecSingValues.asDiagonal() * (-1 * rSVD.matrixV().transpose());
        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_74::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {
        // DEBUG
//    std::cout << "Data to find the embedded value" << std::endl;
//            std::cout << rSVD.matrixU() << std::endl << std::endl
//                      << rSVD.singularValues() << std::endl << std::endl
//                      << rSVD.matrixV() << std::endl << std::endl;
//            std::cout << "The analized matrix" << std::endl;
//            std::cout << (rSVD.matrixU() * rSVD.singularValues().asDiagonal() * rSVD.matrixV().transpose());
//            std::cout << std::endl << std::endl;

        float fZQuantizedValue = std::fmod((float) rSVD.singularValues()(1), _fQModule);
        stBlockData.vecLogoDataBlock[0] += (fZQuantizedValue >= (_fQModule / 2)) ? 0x01 : 0x00;
    }
//#endif
    /* End WMProcess_74 Class */
} /* namespace SProcess */

namespace UProcess {
//#if 0
    /* Start WMProcess_43 Class */
    WMProcess_43::WMProcess_43(unsigned int uiColumns,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : Watermark(uiDimension,
                                                                           uiRedundancy,
                                                                           uiIterations,
                                                                           ((uiDimension - (uiColumns + 1)) * ((uiDimension - (uiColumns + 1)) + 1)) / 2,
                                                                           tyCoordinates),
                                                                 _uiColumns(uiColumns) {

        if (_uiColumns > (uiDimension - 2) ||
            _uiColumns < MIN_PROTECTED_COLUMNS) {
            // TODO Throw exception
        }

        // DEBUG
        std::cout << "Columns    : " << uiColumns << std::endl;
        std::cout << "DataPerBl  : " << _uiDataPerBlock << std::endl;
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_43" << std::endl;
        std::cout << "************************************" << std::endl;
    }

    WMProcess_43::~WMProcess_43() throw() {

    }


    bool WMProcess_43::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_43::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_43::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                               const std::vector<unsigned char>& vecData) {

        unsigned int uiProtected = rSVD.cols() - (_uiColumns + 1);
        if (vecData.size() > (uiProtected * (uiProtected + 1)) / 2) {
            throw;
        }

        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();
        Eigen::MatrixXd matRight = -1 * rSVD.matrixV();

        // DEBUG
//        matLeft.col(2) = -1 * matLeft.col(2);
//        matLeft.col(4) = -1 * matLeft.col(4);
//        matLeft.col(5) = -1 * matLeft.col(5);
//        matLeft.col(6) = -1 * matLeft.col(6);
//        matRight.col(2) = -1 * matRight.col(2);
//        matRight.col(4) = -1 * matRight.col(4);
//        matRight.col(5) = -1 * matRight.col(5);
//        matRight.col(6) = -1 * matRight.col(6);
        std::cout << "Mat Left Base" << std::endl;
        std::cout << matLeft << std::endl;

        unsigned int uiDataIndex = 0;
        for (int iCol = _uiColumns; iCol < rSVD.cols(); iCol++) {
            for (int iRow = 1; iRow < (rSVD.rows() - iCol); iRow++) {

                /*
                 * Change sign of values in column depending on the value that will be stored
                 */
                matLeft(iRow, iCol) = std::abs((float) matLeft(iRow, iCol)) * std::pow(-1, vecData[uiDataIndex]);
                uiDataIndex++;
            }

            // DEBUG
//            std::cout << "Mat Left Post sign sustitution [" << iCol << "]" << std::endl;
//            std::cout << matLeft << std::endl;

            /*
             * Calculate values in order to become the current column linearly independent
             * to the previous columns
             */
            Eigen::MatrixXd linSystemA(iCol, iCol);
            Eigen::VectorXd linSystemB = Eigen::VectorXd::Zero(iCol);

            for (int iCompCol = 0; iCompCol < iCol; iCompCol++) {
                for (int iRow = 0; iRow < rSVD.rows(); iRow++) {

                    if (iRow < (rSVD.rows() - iCol)) {
                        linSystemB(iCompCol) -= matLeft.col(iCol)(iRow) * matLeft.col(iCompCol)(iRow);
                    } else {
                        linSystemA(iCompCol, iRow - (rSVD.rows() - iCol)) = matLeft.col(iCompCol)(iRow);
                    }
                }
            }
            Eigen::VectorXd linSystemX = linSystemA.colPivHouseholderQr().solve(linSystemB);

            // DEBUG
//            std::cout << "Solution" << std::endl;
//            std::cout << "A: " << std::endl
//                      << linSystemA << std::endl
//                      << "B: " << std::endl
//                      << linSystemB << std::endl
//                      << "X: " << std::endl
//                      << linSystemX << std::endl;
            for (int iRow = (rSVD.rows() - iCol); iRow < rSVD.rows(); iRow++) {
                matLeft(iRow, iCol) = linSystemX(iRow - (rSVD.rows() - iCol));
            }

            /*
             * Once stored the specific values, the column vector will be normalized
             */
            matLeft.col(iCol) =  matLeft.col(iCol) / matLeft.col(iCol).norm();
        }

        // DEBUG
        std::cout << uiDataIndex << std::endl;
        std::cout << "Mat Left Post" << std::endl;
        std::cout << matLeft << std::endl;
        std::cout << "Data to made the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) matLeft << std::endl << std::endl
                  << (Eigen::MatrixXd) rSVD.singularValues().asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) matRight << std::endl << std::endl;
        std::cout << "Matrix to be returned" << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose();
        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_43::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {

        // Warning about block selection!!!! Complexity of block
        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();

        // DEBUG
//        std::cout << "Data to find the embedded value" << std::endl;
        std::cout << (Eigen::MatrixXd) matLeft << std::endl << std::endl
                  << (Eigen::MatrixXd) rSVD.singularValues().asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) (-1 * rSVD.matrixV()) << std::endl << std::endl;
//        std::cout << "The analized matrix" << std::endl;
//        std::cout << (matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose());
//        std::cout << std::endl << std::endl;

        unsigned int uiDataIndex = 0;
        for (int iCols = _uiColumns; iCols < (rSVD.cols() - 1); iCols++) {
            for (int iRows = 1; iRows < (rSVD.rows() - iCols); iRows++) {
                stBlockData.vecLogoDataBlock[uiDataIndex] +=
                        (matLeft(iRows, iCols) / std::abs((float) matLeft(iRows, iCols)) > 0) ?
                                0x00 : 0x01;

                // DEBUG
                std::cout << "[" << (float) matLeft(iRows, iCols) << "] " <<
                                    (int) stBlockData.vecLogoDataBlock[uiDataIndex] << std::endl;

                uiDataIndex++;
                if (uiDataIndex == stBlockData.vecLogoDataBlock.size()) {
                    break;
                }
            }
        }
    }
//#endif
    /* End WMProcess_43 Class */
//#if 0
    /* Start WMProcess_09 Class */
    WMProcess_09::WMProcess_09(float fThreshold,
                               unsigned int uiDataPerBlock,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : Watermark(uiDimension,
                                                                           uiRedundancy,
                                                                           uiIterations,
                                                                           uiDataPerBlock,
                                                                           tyCoordinates),
                                                                 _fThreshold(fThreshold) {

        // DEBUG
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_09" << std::endl;
        std::cout << "************************************" << std::endl;
    }
    WMProcess_09::~WMProcess_09() throw() {

    }

    bool WMProcess_09::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_09::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_09::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                              const std::vector<unsigned char>& vecData) {

        // Warning about block selection!!!! Complexity of block
        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();
        Eigen::MatrixXd matRight = -1 * rSVD.matrixV();

        // DEBUG
//        std::cout << "Mat Left Pre" << std::endl;
//        std::cout << matLeft << std::endl;

        // Step 3: check if value whether or not matches with the sign of the
        // difference between u21 and u31
        float _fDifference = std::abs((float) matLeft(1, 0)) - std::abs((float) matLeft(2, 0));
        if ((vecData[0] == 1 && _fDifference >= 0) ||
            (vecData[0] == 0 && _fDifference < 0)) {
            // Assign values in match case
            matLeft(1, 0) = - std::abs((std::abs((float) matLeft(1, 0)) + ((_fThreshold - std::abs(_fDifference)) / 2)));
            matLeft(2, 0) = - std::abs((std::abs((float) matLeft(2, 0)) - ((_fThreshold - std::abs(_fDifference)) / 2)));
        } else {
            // Assign values in unmatch case
            matLeft(1, 0) = - std::abs((std::abs((float) matLeft(1, 0)) - ((_fThreshold + std::abs(_fDifference)) / 2)));
            matLeft(2, 0) = - std::abs((std::abs((float) matLeft(2, 0)) + ((_fThreshold + std::abs(_fDifference)) / 2)));
        }

        // DEBUG
        std::cout << "Data to made the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) matLeft << std::endl << std::endl
                  << (Eigen::MatrixXd) rSVD.singularValues().asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) matRight << std::endl << std::endl;
        std::cout << "Matrix to be returned" << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose();
        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_09::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {
        // Warning about block selection!!!! Complexity of block
        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();
        Eigen::MatrixXd matRight = -1 * rSVD.matrixV();

        // DEBUG
//        std::cout << "Data to find the embedded value" << std::endl;
//        std::cout << matLeft << std::endl << std::endl
//                  << rSVD.singularValues() << std::endl << std::endl
//                  << matRight << std::endl << std::endl;
//        std::cout << "The analized matrix" << std::endl;
//        std::cout << (matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose());
//        std::cout << std::endl << std::endl;

        stBlockData.vecLogoDataBlock[0] += (std::abs((float) matLeft(1, 0)) - std::abs((float) matLeft(2, 0)) >= 0) ? 0x01 : 0x00;
    }
//#endif
    /* End WMProcess_09 Class */
} /* namespace UProcess */

namespace UVProcess {
//#if 0
    /* Start WMProcess_18 Class */
    WMProcess_18::WMProcess_18(float fThreshold,
                               unsigned int uiDimension,
                               unsigned int uiRedundancy,
                               unsigned int uiIterations,
                               Type_Coordinates tyCoordinates) : UProcess::WMProcess_09(fThreshold,
                                                                                        DATA_SIZE_PER_BLOCK_18,
                                                                                        uiDimension,
                                                                                        uiRedundancy,
                                                                                        uiIterations,
                                                                                        tyCoordinates) {

        /*
         * This parameter must be initialized by this way due to 09 does not have a parameter
         * to handle this initialization
         */
        _uiDataPerBlock = 2;

        // DEBUG
        std::cout << "************************************" << std::endl;
        std::cout << "WMProcess_18" << std::endl;
        std::cout << "************************************" << std::endl;
    }
    WMProcess_18::~WMProcess_18() throw() {

    }

    bool WMProcess_18::isEmbeddable(const IImageData& hostImage,
                                    const IImageData& logoImage) {

        return true;
    }

    bool WMProcess_18::canBeExtracted(const IImageData& logoOutputImage,
                                      const IImageData& watermarkedImage) {

        return true;
    }

    Eigen::MatrixXd WMProcess_18::getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                               const std::vector<unsigned char>& vecData) {

        UProcess::WMProcess_09::getWMedBlock(rSVD,
                                             vecData);
        // Warning about block selection!!!! Complexity of block
        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();
        Eigen::MatrixXd matRight = -1 * rSVD.matrixV();

        // DEBUG
//        std::cout << "Mat Right Pre" << std::endl;
//        std::cout << matRight << std::endl;

        // Step 4: check if value whether or not matches with the sign of the
        // difference between u13 and u12
        float _fDifference = std::abs((float) matRight(1, 0)) - std::abs((float) matRight(2, 0));
        if ((vecData[1] == 1 && _fDifference >= 0) ||
            (vecData[1] == 0 && _fDifference < 0)) {
            // Assign values in match case
            matRight(1, 0) = - std::abs((std::abs((float) matRight(1, 0)) + ((_fThreshold - std::abs(_fDifference)) / 2)));
            matRight(2, 0) = - std::abs((std::abs((float) matRight(2, 0)) - ((_fThreshold - std::abs(_fDifference)) / 2)));
        } else {
            // Assign values in unmatch case
            matRight(1, 0) = - std::abs((std::abs((float) matRight(1, 0)) - ((_fThreshold + std::abs(_fDifference)) / 2)));
            matRight(2, 0) = - std::abs((std::abs((float) matRight(2, 0)) + ((_fThreshold + std::abs(_fDifference)) / 2)));
        }

        // DEBUG
        std::cout << "Data to made the final matrix up w/ workaround" << std::endl;
        std::cout << (Eigen::MatrixXd) matLeft << std::endl << std::endl
                  << (Eigen::MatrixXd) rSVD.singularValues().asDiagonal() << std::endl << std::endl
                  << (Eigen::MatrixXd) matRight << std::endl << std::endl;
        std::cout << "Matrix to be returned" << std::endl;

        /*
         * Workaround to get the expected signs in both U and V matrices. They both
         * will be multiplied by -1
         */
        Eigen::MatrixXd matWorkAround = matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose();
        matWorkAround.row(0).swap(matWorkAround.row(1));

        // DEBUG
        std::cout << matWorkAround << std::endl << std::endl;

        return matWorkAround;
    }

    void WMProcess_18::extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                   BlockData&  stBlockData) {
        // Warning about block selection!!!! Complexity of block
        Eigen::MatrixXd matLeft = -1 * rSVD.matrixU();
        Eigen::MatrixXd matRight = -1 * rSVD.matrixV();

        // DEBUG
//        std::cout << "Data to find the embedded value" << std::endl;
//        std::cout << matLeft << std::endl << std::endl
//                  << rSVD.singularValues() << std::endl << std::endl
//                  << matRight << std::endl << std::endl;
//        std::cout << "The analized matrix" << std::endl;
//        std::cout << (matLeft * rSVD.singularValues().asDiagonal() * matRight.transpose());
//        std::cout << std::endl << std::endl;

        stBlockData.vecLogoDataBlock[0] += (std::abs((float) matLeft(1, 0)) - std::abs((float) matLeft(2, 0)) >= 0) ? 0x01 : 0x00;
    }
//#endif
    /* End WMProcess_18 Class */
} /* namespace UVProcess */

namespace USVProcess {

} /* namespace USVProcess */
