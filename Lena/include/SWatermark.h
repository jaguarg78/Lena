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
               int iDelta = 0,
               unsigned int iRedundancy = 1,
               unsigned int iIterations = 1,
               Type_Coordinates tyCoordinates = TYPE_ADJACENT);
    virtual ~SWatermark() throw();

    virtual void progressInsertion() {}
    virtual void progressExtraction() {}

protected:
    virtual bool isEmbeddable(const IImageData& hostImage,
                              const IImageData& logoImage);
    virtual Eigen::MatrixXd processBlock(const Eigen::MatrixXd& mBlock,
                                         unsigned char ucPixelBitValue);
    int     _iDelta;
private:
    SWatermark();
    SWatermark(const SWatermark& rSource);
};

#endif /* SRC_SWATERMARK_H_ */
