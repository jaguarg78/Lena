/*
 * Utilities.h
 *
 *  Created on: Feb 5, 2016
 *      Author: gianni
 */

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <cmath>

#include "definitions/Global.h"
#include "IImageData.h"

class IImageData;

class Utilities {
public:
    static double MSE_RGB(const IImageData& rBMPRawImage,
                          const IImageData& rBMPModifiedImage);
    static double PSNR_RGB(const IImageData& rBMPRawImage,
                           const IImageData& rBMPModifiedImage);

    static double BCR(const IImageData& rBMPInputLogo,
                      const IImageData& rBMPOutputLogo);
private:
    Utilities();
    virtual ~Utilities();

    Utilities(const Utilities& source);
    Utilities& operator=(const Utilities& source);
};

#endif /* SRC_UTILITIES_H_ */
