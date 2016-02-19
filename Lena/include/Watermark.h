/*
 * Watermark.h
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#ifndef SRC_WATERMARK_H_
#define SRC_WATERMARK_H_

#include <climits>

#include <eigen3/Eigen/Dense>

#include "IImageData.h"

#include "definitions/Global.h"

class Watermark {
public:
    enum Type_Coordinates {
        TYPE_ADJACENT,
        TYPE_RANDOM,
        TYPE_CODED
    };

	Watermark(int iWidth,
              int iHeight,
              unsigned int iRedundancy,
              unsigned int iIterations,
              Type_Coordinates tyCoordinates);
	virtual ~Watermark() throw();

	virtual void progressInsertion() = 0;
	virtual void progressExtraction() = 0;

	int insertLogo(const IImageData& hostImage,
                   const IImageData& logoImage);
	int extractLogo();
	void showProgress();

//	void setWidth(int iWidth);
//	void setHeight(int iHegiht);
//	void setInputImageData(unsigned char *pucInputImageData);
//	void setInputLogoData(unsigned char *pucInputLogoData);
//	void setOutputImageData(unsigned char *pucOutputImageData);
//	void setOutputLogoData(unsigned char *pucOutputLogoData);
protected:
	struct InsertionData {
        unsigned char   ucPixelBitValue; // Value to be embedded
        unsigned int    uiVBlock;         // V Coordinate of logo image
        unsigned int    uiHBlock;         // H Coordinate of logo image in bytes
    };

	virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage) = 0;
	virtual Eigen::MatrixXd processBlock(const Eigen::MatrixXd& mBlock,
	                                     unsigned char ucPixelBitValue) = 0;

	int				 _iWidth;
	int				 _iHeight;
	unsigned int     _iRedundancy;
	unsigned int     _iIterations;
	unsigned char 	*_pucInputImageData;
//	unsigned char 	*_pucInputLogoData;
	unsigned char 	*_pucOutputImageData;
	unsigned char 	*_pucOutputLogoData;

	Type_Coordinates _tyCoordinates;
private:
	Watermark();
	Watermark(const Watermark& rSource);

	void insertProcess(const IImageData& hostImage,
	                   const InsertionData& stInsertionData);
	Eigen::MatrixXd getBlockData(const IImageData& hostImage,
                                 const InsertionData& stInsertionData);
    void setBlockData(IImageData& hostImage,
                      const InsertionData& stInsertionData,
                      const Eigen::MatrixXd& mWMBlock);
};

#endif /* SRC_WATERMARK_H_ */
