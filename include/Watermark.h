/*
 * Watermark.h
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#ifndef SRC_WATERMARK_H_
#define SRC_WATERMARK_H_

#include "IImageData.h"

class Watermark {
public:
    enum Type_Coordinates {
        TYPE_DIMENSIONS,
        TYPE_ADJACENT,
        TYPE_RANDOM,
        TYPE_CODED
    };

	Watermark(unsigned int uiDimension,
              unsigned int uiRedundancy,
              unsigned int uiIterations,
              unsigned int uiDataPerBlock,
              Type_Coordinates tyCoordinates);
	virtual ~Watermark() throw();

	int embedLogo(IImageData&       hostImage,
                  const IImageData& logoImage);
	int extractLogo(IImageData&       logoOutputImage,
                    const IImageData& watermarkedImage);
	void showProgress();

protected:
	struct BlockData {
        std::vector<unsigned char>  vecLogoDataBlock;  // Vector of values to be embedded per block
        unsigned int                uiVBlock;          // V Coordinate of logo image
        unsigned int                uiHBlock;          // H Coordinate of logo image in bytes
    };

	/* Pure virtual functions */
	virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage) = 0;
	virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                const IImageData& watermarkedImage) = 0;
	virtual Eigen::MatrixXd getWMedBlock(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
                                         const std::vector<unsigned char>& vecData) = 0;
	virtual void extractData(const Eigen::JacobiSVD<Eigen::MatrixXd>& rSVD,
	                         BlockData&  stBlockData) = 0;

    unsigned int	 _uiDimension;
	unsigned int     _uiDataPerBlock;
	unsigned int     _uiRedundancy;
	unsigned int     _uiIterations;

	Type_Coordinates _tyCoordinates;

private:
	Watermark();
	Watermark(const Watermark& rSource);

	void embeddingProcess(IImageData&          hostImage,
	                      const BlockData&     stBlockData);
	Eigen::MatrixXd getBlockMatrix(const IImageData& hostImage,
                                   const BlockData&  stBlockData);
    void setBlockData(IImageData&            hostImage,
                      const BlockData&       stBlockData,
                      const Eigen::MatrixXd& mWMBlock);
    void pupulateBlockIndexes(BlockData&        stBlockData,
                              const IImageData& hostImage,
                              int               iBlockCounter,
                              Type_Coordinates  typeCoordinates = TYPE_ADJACENT);
    void getEmbeddedDataPerBlock(const IImageData& watermarkedImage,
                                 BlockData&  stBlockData);
};

#endif /* SRC_WATERMARK_H_ */
