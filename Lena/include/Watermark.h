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
	Watermark(int iWidth,
              int iHeight,
              unsigned int iRedundancy,
              unsigned int iIterations);
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
	virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage) = 0;
	virtual void insertProcess(const IImageData& hostImage,
                               unsigned char ucPixelBitValue,
                               unsigned int uiVLogo,
                               unsigned int uiHLogoByte,
                               unsigned short usBitPixelIndex) = 0;
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
private:
	Watermark();
	Watermark(const Watermark& rSource);
};

#endif /* SRC_WATERMARK_H_ */
