/*
 * Watermark.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#include "Watermark.h"
#include "Utilities.h"

Watermark::Watermark(unsigned int uiDimension,
                     unsigned int uiRedundancy,
                     unsigned int uiIterations,
                     unsigned int uiDataPerBlock,
                     Type_Coordinates tyCoordinates) : _uiDimension(uiDimension),
                                                       _uiDataPerBlock(uiDataPerBlock),
                                                       _uiRedundancy(uiRedundancy),
                                                       _uiIterations(uiIterations),
                                                       _tyCoordinates(tyCoordinates) {
    double dLogDimensionBase2 = std::log(_uiDimension) / std::log(2);
    if (_uiDimension < BLOCK_DIMENSION_DEFAULT ||
        std::floor(dLogDimensionBase2) != dLogDimensionBase2) {
        // TODO Throw exception
    }

    if (_uiRedundancy < 1 || _uiRedundancy % 2 == 0) {
        // TODO Throw exception
    }

    if (_uiIterations < 1 || _uiIterations % 2 == 0) {
        // TODO Throw exception
    }

    // DEBUG
    std::cout << "***********************************" <<std::endl;
    std::cout << "***********************************" <<std::endl;
    std::cout << "Dimensions : " << _uiDimension << std::endl;
    std::cout << "Data/block : " << _uiDataPerBlock << std::endl;
    std::cout << "Redundancy : " << _uiRedundancy << std::endl;
    std::cout << "Iterations : " << _uiIterations << std::endl;
}

Watermark::~Watermark() throw() {

}

int Watermark::embedLogo(IImageData&       hostImage,
                         const IImageData& logoImage) {
    /*
     * Check validation conditions for each embedding process
     */
    if (!isEmbeddable(hostImage,
                      logoImage)) {
        // TODO throw exception!!!!!
    }

    /*
     * Structure to indicate the number of complete blocks and remaining
     * info to be processed for the complete logo data
     */
    int iSizeToBeEmbedded = logoImage.getHeight() * logoImage.getWidth();
    std::div_t divGlobalBlocksInfo = std::div(iSizeToBeEmbedded, _uiDataPerBlock);

    // DEBUG
//    std::cout << "Global Div : " << divGlobalBlocksInfo.quot << "/" << divGlobalBlocksInfo.rem << std::endl;

    /*
     * Structure to store information to be used at embedding process for each block
     */
    BlockData    stInsertionData;
    /*
     * Variable used to stored data into the proper block
     */
    int iBlockCounter(0);

    /*
     * The logo image will be traversed in order to embed each pixel into the host image
     * depending on each algorithm
     *
     * uiVLogo : Vertical coordinate of binary image logo
     * uiHLogoByte : Horizontal coordinate of binary image logo in bytes
     */
    for (int uiVLogo = 0;
             uiVLogo < logoImage.getHeight();
             uiVLogo++) {

        /*
         * Lambda function used to stop iteration when last bit of the row
         * of data has been already embedded
         */
        [&] {
            for (unsigned int uiHLogoByte = 0;
                              uiHLogoByte < logoImage.getRowSizeFixed();
                              uiHLogoByte++) { // H Coordinate of logo image in bytes

                /* Byte Index into binary image logo Data */
                unsigned int uiByteIndex = (uiVLogo * logoImage.getRowSizeFixed()) +
                                           uiHLogoByte;
                /* Byte Info that will be embedded */
                unsigned char ucLogoByte = logoImage.getConstData()[uiByteIndex];

                /* Pixel by byte */
                for (unsigned short usBitPixelIndex = 0;
                                    usBitPixelIndex < CHAR_BIT;
                                    usBitPixelIndex++) {

                    /*
                     * uiCurrentRowBit : Value associated to the data value per row.
                     * Excludes padding info
                     */
                    int uiCurrentRowBit = (uiHLogoByte * CHAR_BIT) + usBitPixelIndex;
                    if (uiCurrentRowBit == logoImage.getWidth()) {
                        /*
                         * Returns from lambda function.
                         */
                        return;
                    }

                    /*
                     * Extracting the specific bit value from byte data
                     */
                    unsigned char ucPixelBitValue = (ucLogoByte & 0x80) >> 0x07;
                    ucLogoByte <<= 1;

                    // DEBUG
//                    std::cout << stInsertionData.vecLogoDataBlock.size() << " [" << usBitPixelIndex << "] " << (int) ucPixelBitValue << std::endl;

                    /*
                     * Structure to indicate the blocks where the current bit will be stored
                     */
                    std::div_t divCurrentBitBlockInfo = std::div((int) ((uiVLogo * logoImage.getWidth()) + uiCurrentRowBit),
                                                            _uiDataPerBlock);
                    /* usBitRedundancy : Indicates how many times the value will be stored */
                    for (unsigned int usBitRedundancy = 0;
                                      usBitRedundancy < _uiRedundancy;
                                      usBitRedundancy++) {
                        // DEBUG
//                        std::cout << "*************" << std::endl;
//                        std::cout << "GQuot : " << divGlobalBlocksInfo.quot << std::endl;
//                        std::cout << "GRem  : " << divGlobalBlocksInfo.rem << std::endl;
//                        std::cout << "CQuot : " << divCurrentBitBlockInfo.quot << std::endl;
//                        std::cout << "CRem  : " << divCurrentBitBlockInfo.rem << std::endl;
//                        std::cout << "Size1 : " << stInsertionData.vecLogoDataBlock.size() << std::endl;
//                        std::cout << "CRB   : " << uiCurrentRowBit << std::endl;

                        // DEBUG
//                        std::cout << "[" << uiVLogo << "," << uiHLogoByte << "," << usBitPixelIndex << "]" << std::endl;


                        /*
                         * vecLogoDataBlock : Vector with data to be embedded
                         */
                        stInsertionData.vecLogoDataBlock.push_back(ucPixelBitValue);

                        // DEBUG
//                        std::cout << "[" << stInsertionData.vecLogoDataBlock.size() << "]" << std::endl;

                        /*
                         * If Size of data to be embedded is multiple of data per block:(1)
                         *      Just push back the bit info to the  vector, once the complete information is
                         *      stored, it will be processed.
                         * Otherwise, when size of data to be embedded is not multiple of data per
                         * block: (most of cases) (2)
                         *      Validates if bit index will be in the last block of embedded info. (3)
                         *          In that case just wait to stored the complete info to the vector (4)
                         *          Otherwise, stored only the remaining info (5)
                         */
                        if ((divGlobalBlocksInfo.rem == 0 &&
                                (((uiCurrentRowBit * _uiRedundancy) + usBitRedundancy + 1) % _uiDataPerBlock == 0)) ||            // (1)
                            (divGlobalBlocksInfo.rem > 0 &&                                                                       // (2)
                                    (((divCurrentBitBlockInfo.quot < divGlobalBlocksInfo.quot) &&                                 // (3)
                                            stInsertionData.vecLogoDataBlock.size() == _uiDataPerBlock) ||                        // (4)
                                     ((divCurrentBitBlockInfo.quot == divGlobalBlocksInfo.quot) &&                                // (3)
                                            stInsertionData.vecLogoDataBlock.size() == (unsigned) divGlobalBlocksInfo.rem)))) {   // (5)

                            // DEBUG
//                            std::cout << "*************" << std::endl;
//                            std::cout << "GQuot : " << divGlobalBlocksInfo.quot << std::endl;
//                            std::cout << "GRem  : " << divGlobalBlocksInfo.rem << std::endl;
//                            std::cout << "CQuot : " << divCurrentBitBlockInfo.quot << std::endl;
//                            std::cout << "CRem  : " << divCurrentBitBlockInfo.rem << std::endl;
//                            std::cout << "Size1 : " << stInsertionData.vecLogoDataBlock.size() << std::endl;
//                            std::cout << "CRB   : " << uiCurrentRowBit << std::endl;
                            /*
                             * usProcessIteration : Indicates how many times the embedding process
                             * will be repeated on the same block
                             */
                            for (unsigned int usProcessIteration = 0;
                                              usProcessIteration < _uiIterations;
                                              usProcessIteration++) {

                                /*
                                 * The following attributes will be populated
                                 * uiVBlock : Vertical index of the block where bit value will be stored
                                 * uiHBlock : Horizontal index of the block where bit value will be stored
                                 */
                                pupulateBlockIndexes(stInsertionData,
                                                     hostImage,
                                                     iBlockCounter);


                                // DEBUG
                                std::cout << "[" << stInsertionData.uiVBlock << ", "
                                                 << stInsertionData.uiHBlock << "] "
                                                 << stInsertionData.vecLogoDataBlock.size() << std::endl;
//                                std::cout << Utilities::Debug::getValuesFromVector(
//                                                         stInsertionData.vecLogoDataBlock,
//                                                         (divCurrentBitBlockInfo.quot * _uiDataPerBlock) + usBitRedundancy, // CHECK THIS
//                                                         logoImage.getWidth() * _uiRedundancy);
//#if 0
                                embeddingProcess(hostImage,
                                                 stInsertionData);
//#endif
                                iBlockCounter++;
                            } // usProcessIteration

                            stInsertionData.vecLogoDataBlock.clear();
                            // DEBUG
//                            std::cout << "--[" << stInsertionData.vecLogoDataBlock.size() << "]" << std::endl;
                        }
                    } // usBitRedundancy
                } // usBitPixelIndex
            } // uiHLogoByte
        }(); // Lambda function
    } // uiVLogo

    // DEBUG
    std::cout << "Creating output File: " << hostImage.getFileName() << std::endl;
    hostImage.create();

	return 0;
}

int Watermark::extractLogo(IImageData&       logoOutputImage,
                           const IImageData& watermarkedImage) {


    // DEBUG
    std::cout << "****** Extracting ****" <<std::endl;

    /*
     * Check validation conditions for each extracting process
     */
    if (!canBeExtracted(logoOutputImage,
                        watermarkedImage)) {
        // TODO throw exception!!!!!
    }

    /*
     * Structure to indicate the number of complete blocks and remaining
     * info to be processed for the complete logo data
     */
    int iSizeToBeExtracted = logoOutputImage.getHeight() * logoOutputImage.getWidth();
    std::div_t divGlobalBlocksInfo = std::div(iSizeToBeExtracted, _uiDataPerBlock);
    /*
     * Structure to indicate the padding associated to the extra information
     * added in order to get 4-byte-fit
     */
    std::div_t divBytesPerRowInfo = std::div((int) logoOutputImage.getWidth(), CHAR_BIT);

    /*
     * Calculation to know how many blocks were used to store complete information.
     */
    int iBlocksUsed = std::ceil(
                            (double) logoOutputImage.getHeight() * logoOutputImage.getWidth() * _uiRedundancy /
                            (double) _uiDataPerBlock) *
                            _uiIterations;

//    // DEBUG
//    std::cout << "Blocks used : " << iBlocksUsed << std::endl
//              << "GQuo : " << divGlobalBlocksInfo.quot << std::endl
//              << "GRem : " << divGlobalBlocksInfo.rem << std::endl;

    /*
     * Structure to store information to be used at embedding process for each block
     */
    BlockData    stExtractionData;
    stExtractionData.vecLogoDataBlock.resize(_uiDataPerBlock);

    /*
     * The following variables were declared otherwise as a loop, because
     * of in some processes the bytes could be split in different blocks of
     * information
     */
    /* Pixel by byte */
    unsigned short usBitPixelIndex = 0;
    /* Byte Info that will be extracted */
    unsigned char ucLogoByte = 0x0;
    /* Byte offset */
    unsigned int uiByteOffset = 0;

    /*
     * usBitRedundancy : Indicates how many times the value will be stored
     * usRedundancyValue : Accumulator of redundancy values.
     */
    unsigned int usBitRedundancy = 0;
    unsigned short usRedundancyValue = 0;
    for (int iEmbeddedBlock = 0;
             iEmbeddedBlock < iBlocksUsed;
             iEmbeddedBlock += _uiIterations) {

        /*
         * Just for last block the size will be set to the remaining info.
         * TODO: Check if modularity = 0 does not affect this validation
         */
        if (iEmbeddedBlock == (iBlocksUsed - 1)) {
            stExtractionData.vecLogoDataBlock.clear();
            stExtractionData.vecLogoDataBlock.resize(divGlobalBlocksInfo.rem);
        }

        /* Reset values to 0 for each value in vector */
        std::fill(stExtractionData.vecLogoDataBlock.begin(),
                  stExtractionData.vecLogoDataBlock.end(),
                  0);

        /*
         * usProcessIteration : Indicates how many times the embedding process
         * were repeated on the same block
         */
        for (unsigned int usProcessIteration = 0;
                          usProcessIteration < _uiIterations;
                          usProcessIteration++) {

            /*
             * The following attributes will be populated
             * uiVBlock : Vertical index of the block where bit value will be stored
             * uiHBlock : Horizontal index of the block where bit value will be stored
             */
            pupulateBlockIndexes(stExtractionData,
                                 watermarkedImage,
                                 iEmbeddedBlock + usProcessIteration);



            getEmbeddedDataPerBlock(watermarkedImage,
                                    stExtractionData);
        }


        for (unsigned int i = 0; i < stExtractionData.vecLogoDataBlock.size(); i++) {

            /*
             * Set final value after analyze all iterations.
             *      1 if addition results greater than half
             *      0 otherwise.
             */
            stExtractionData.vecLogoDataBlock[i] =
                    (stExtractionData.vecLogoDataBlock[i] > (_uiIterations / 2)) ?
                            0x01 : 0x00;
            // DEBUG
            std::cout << (int) stExtractionData.vecLogoDataBlock[i] << " ";

            /* Calculating final value averaging the redundant values */
            usRedundancyValue += stExtractionData.vecLogoDataBlock[i];
            usBitRedundancy++;
            /*
             * Process used to get right values, no matter if redundancy are
             * split into several blocks
             */
            if (usBitRedundancy < _uiRedundancy) {
                continue;
            }
            /* Reset redundancy counter */
            usBitRedundancy = 0;
            usRedundancyValue = (usRedundancyValue > (_uiRedundancy / 2)) ?
                                    0x01 : 0x00;

            // DEBUG
            std::cout << "|" << usRedundancyValue << " ";

            ucLogoByte = ucLogoByte | ((usRedundancyValue & 0x01) << (CHAR_BIT - (usBitPixelIndex + 1)));
            usBitPixelIndex++;

            int iRowDataDiff = uiByteOffset % logoOutputImage.getRowSizeFixed();
            if ((iRowDataDiff != divBytesPerRowInfo.quot &&
                    usBitPixelIndex == CHAR_BIT) ||
                (iRowDataDiff == divBytesPerRowInfo.quot &&
                    usBitPixelIndex == divBytesPerRowInfo.rem)) {

                std::memcpy((char *) logoOutputImage.getData() + uiByteOffset,
                            &ucLogoByte,
                            sizeof(unsigned char));

                usBitPixelIndex = 0;
                ucLogoByte = 0x00;

                if (iRowDataDiff != divBytesPerRowInfo.quot) {
                    if (divBytesPerRowInfo.rem != 0 ||
                        (divBytesPerRowInfo.rem == 0 &&
                         iRowDataDiff < (divBytesPerRowInfo.quot - 1))) {

                        uiByteOffset++;
                    } else if (divBytesPerRowInfo.rem == 0 &&
                               iRowDataDiff == (divBytesPerRowInfo.quot - 1)) {

                        uiByteOffset += logoOutputImage.getRowSizeFixed() - iRowDataDiff;
                    }
                } else {
                    uiByteOffset += logoOutputImage.getRowSizeFixed() - iRowDataDiff;
                }
            }

            usRedundancyValue = 0;
        }
        // DEBUG
        std::cout << std::endl;

        // DEBUG
        std::cout << "Block Data: " << std::endl;
        std::cout << "*[" << iEmbeddedBlock << "] ";
        std::cout << "-[" << (int) stExtractionData.vecLogoDataBlock[0] << "] ";
        std::cout << " [" << stExtractionData.uiVBlock << ", "
                          << stExtractionData.uiHBlock << "] "
                          << stExtractionData.vecLogoDataBlock.size() << std::endl;
    }

    // DEBUG
    std::cout << "Blocks used : " << iBlocksUsed << std::endl
              << "GQuo : " << divGlobalBlocksInfo.quot << std::endl
              << "GRem : " << divGlobalBlocksInfo.rem << std::endl;

    // DEBUG
    std::cout << "******************* Data  ******************" << std::endl;
    for (unsigned int i = 0; i < logoOutputImage.getDataSize(); i++) {
        if (i % logoOutputImage.getRowSizeFixed() == 0) {
            std::cout << std::endl;
        }

        unsigned char ucByteValue = logoOutputImage.getData()[i];
        for (int j = 0; j < CHAR_BIT; j++) {
            std::cout << (int) ((ucByteValue & 0x80) >> 7) << " ";
            ucByteValue <<= 0x01;
        }
    }

    return 0;

}

void Watermark::embeddingProcess(IImageData&        hostImage,
                                 const BlockData&   stBlockData) {
    //DEBUG
//    std::cout << "[" << stInsertionData.uiVBlock << " "
//                     << stInsertionData.uiHBlock << "] " << std::endl;

    /* Create blocks where bits will be embedded */
    Eigen::MatrixXd mBlock = getBlockMatrix(hostImage,
                                            stBlockData);

    // DEBUG
    std::cout << mBlock << std::endl << std::endl;

    /* Calculate matrix decomposition to be embedded */
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(mBlock, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::MatrixXd mWMBlock = getWMedBlock(svd,
                                            stBlockData.vecLogoDataBlock);
    Utilities::normalizeValues(mWMBlock);

    // DEBUG
    std::cout << mWMBlock << std::endl << std::endl;

    /* Embedded data insertion */
    setBlockData(hostImage,
                 stBlockData,
                 mWMBlock);
}

Eigen::MatrixXd Watermark::getBlockMatrix(const IImageData& hostImage,
                                          const BlockData&  stBlockData) {

    Eigen::MatrixXd mBlock(_uiDimension, _uiDimension);

    for (unsigned int iBlockRow = 0; iBlockRow < _uiDimension; iBlockRow++) {
        for (unsigned int iBlockCol = 0; iBlockCol < _uiDimension; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((stBlockData.uiVBlock * _uiDimension) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((stBlockData.uiHBlock * _uiDimension) + iBlockCol))) * sizeof(Pixel));


            //DEBUG
//            std::cout << "Offset: " << uiDataOffset << std::endl;

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
                oss << "Block [" << stBlockData.uiVBlock << "," << stBlockData.uiHBlock << "]"
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

void Watermark::setBlockData(IImageData&            hostImage,
                             const BlockData&       stInsertionData,
                             const Eigen::MatrixXd& mWMBlock) {

    for (unsigned int iBlockRow = 0; iBlockRow < _uiDimension; iBlockRow++) {
        for (unsigned int iBlockCol = 0; iBlockCol < _uiDimension; iBlockCol++) {
            /* Offset from host Data pointer */
            unsigned int uiDataOffset = ((((stInsertionData.uiVBlock * _uiDimension) + iBlockRow) * hostImage.getRowSizeFixed()) +
                                        ((((stInsertionData.uiHBlock * _uiDimension) + iBlockCol))) * sizeof(Pixel));

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

void Watermark::pupulateBlockIndexes(BlockData&        stBlockData,
                                     const IImageData& hostImage,
                                     int               iBlockCounter,
                                     Type_Coordinates  typeCoordinates) {

    switch (typeCoordinates) {
    case TYPE_ADJACENT: {
        div_t divIndex = std::div((int) iBlockCounter,
                                  std::floor((double) hostImage.getWidth() / (double) _uiDimension));
        stBlockData.uiVBlock = divIndex.quot;
        stBlockData.uiHBlock = divIndex.rem;
        break;
    }
    case TYPE_RANDOM:
        break;
    case TYPE_CODED:
        break;
    default:
        break;
    }
}

void Watermark::getEmbeddedDataPerBlock(const IImageData& watermarkedImage,
                                        BlockData&  stBlockData) {

    /* Get Watermarked block where bits will be embedded */
    Eigen::MatrixXd mBlock = getBlockMatrix(watermarkedImage,
                                            stBlockData);

    //DEBUG
    std::cout << mBlock << std::endl;
    std::cout << std::endl;

    /* Calculate embedded decomposition */
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(mBlock, Eigen::ComputeFullU | Eigen::ComputeFullV);
    extractData(svd, stBlockData);
}

void Watermark::showProgress() {

}
