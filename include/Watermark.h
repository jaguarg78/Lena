/*
 * Watermark.h
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#ifndef SRC_WATERMARK_H_
#define SRC_WATERMARK_H_

#include "IImageData.h"
#include "SVD.h"

typedef Lena::SVD tSVD;
//typedef Eigen::JacobiSVD<Eigen::MatrixXd>	tSVD;

class Watermark {
public:
    typedef enum eCoordinates {
        TYPE_DIMENSIONS,
        TYPE_ADJACENT,
        TYPE_RANDOM,
        TYPE_CODED
    } Coordinates;

    Watermark(unsigned int uiDimension,
            unsigned int uiRedundancy,
            unsigned int uiIterations,
            unsigned int uiDataPerBlock,
            Coordinates  tCoordinates);
    virtual ~Watermark() throw ();

    int embedLogo(IImageData& hostImage,
            const IImageData& logoImage);
    int extractLogo(IImageData& logoOutputImage,
            const IImageData& watermarkedImage);
    void showProgress();

protected:
    struct BlockData {
        std::vector<unsigned char> vecLogoDataBlock; // Vector of values to be embedded per block
        unsigned int uiVBlock; // V Coordinate of logo image
        unsigned int uiHBlock; // H Coordinate of logo image in bytes
        bool hasDebugInfo;
        std::pair<int, int> pairDebugV;
        std::pair<int, int> pairDebugH;
    };

    /* Pure virtual functions */
    virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage) = 0;
    virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                const IImageData& watermarkedImage) = 0;
    virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                         const std::vector<unsigned char>& vecData) = 0;
    virtual void extractData(const tSVD& rSVD,
                             BlockData&  stBlockData) = 0;

    unsigned int _uiDimension;
    unsigned int _uiDataPerBlock;
    unsigned int _uiRedundancy;
    unsigned int _uiIterations;

    Coordinates  _tCoordinates;

private:
    Watermark();
    Watermark(const Watermark& rSource);

    void embeddingProcess(IImageData& hostImage,
                          const BlockData& stBlockData);
    Eigen::MatrixXd getBlockMatrix(const IImageData& hostImage,
                                   const BlockData& stBlockData);
    void setBlockData(IImageData& hostImage,
                      const BlockData& stBlockData,
                      const Eigen::MatrixXd& mWMBlock);
    void pupulateBlockIndexes(BlockData& stBlockData,
                              const IImageData& hostImage,
                              int iBlockCounter,
                              Coordinates coordinates = TYPE_ADJACENT);
    void getEmbeddedDataPerBlock(const IImageData& watermarkedImage,
                                 BlockData& stBlockData);
};

#endif /* SRC_WATERMARK_H_ */
