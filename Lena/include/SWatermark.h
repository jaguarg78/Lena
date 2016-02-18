/*
 * SWatermark.h
 *
 *  Created on: Feb 9, 2016
 *      Author: gianni
 */

#ifndef SRC_SWATERMARK_H_
#define SRC_SWATERMARK_H_

#include "Watermark.h"

class SWatermark: public Watermark {
public:
    SWatermark(int iWidth,
               int iHeight,
               int iDelta = 0);
    virtual ~SWatermark() throw();

    virtual void progressInsertion() {}
    virtual void progressExtraction() {}

protected:
    virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage);
    virtual void insertProcess(const IImageData& hostImage,
                               unsigned char ucPixelBitValue,
                               unsigned int uiVLogo,
                               unsigned int uiHLogoByte,
                               unsigned short usLogoPixel);
    virtual Eigen::MatrixXd processBlock(const Eigen::MatrixXd& mBlock,
                                         unsigned char ucPixelBitValue);
    int     _iDelta;
private:
    SWatermark();
    SWatermark(const SWatermark& rSource);

    Eigen::MatrixXd getBlockData(unsigned int uiVBlock,
                                 unsigned int uiHBlock,
                                 const IImageData& hostImage);

    void setBlockData(unsigned int uiVBlock,
                      unsigned int uiHBlock,
                      IImageData& hostImage,
                      const Eigen::MatrixXd& mWMBlock);
};

#endif /* SRC_SWATERMARK_H_ */
