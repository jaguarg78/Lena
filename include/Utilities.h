/*
 * Utilities.h
 *
 *  Created on: Feb 5, 2016
 *      Author: gianni
 */

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <cmath>
#include <ostream>

#include "IImageData.h"

extern "C" {
    /* DGESVD prototype */
    extern void dgesvd_(char*   pcJobU,     // Defines the computing options of U
                        char*   pcJobVt,    // Defines the computing options of U
                        int*    piRows,     // Number of Matrix' Rows (m)
                        int*    piColumns,  // Number of Matrix' Columns (n)
                        double* pdMatrixA,  // m x n Matrix A as an array
                        int*    piLeadA,    // Leading Dimension of A (m)
                        double* pdMatrixS,  // m Vector S
                        double* pdMatrixU,  // m x n Matrix U as an array
                        int*    piLeadV,    // Leading Dimension of U (n)
                        double* pdMatrixV,  // m x n Matrix Vt as an array
                        int*    piLeadU,    // Leading Dimension of V (m)
                        double* pdWork,     // Workspace as an array
                        int*    piLeadWS,   // LD of Workspace >= max(3*min(m,n)+max(m,n), 5*min(m,n)).
                                            // -1 calculates optimal Workspace size.
                        int*    piInfo);    // 0 Success.
                                            // < 0 = -j. jth parameter argument illegal value
                                            // > 0. Algorithm did not converge
}

class IImageData;

namespace Utilities {
    enum GRAY_CALC {
        GRAY_MEAN,
        GRAY_LIGHTNESS,
        GRAY_LUMINOSITY
    };

    double MSE_RGB(const IImageData& rBMPRawImage,
                   const IImageData& rBMPModifiedImage);
    double PSNR_RGB(const IImageData& rBMPRawImage,
                    const IImageData& rBMPModifiedImage);
    double BCR(const IImageData& rBMPInputLogo,
               const IImageData& rBMPOutputLogo);
    double BCR(const std::vector<unsigned char>& inputData,
               const std::vector<unsigned char>& outputData,
               unsigned int *puiDimension = NULL,
               unsigned int *puiPaddingBytes = NULL);

    void toGrayScale(IImageData& rBMPRawImage,
                     GRAY_CALC eGrayCalc = GRAY_LUMINOSITY);
    SVD getSVD(Eigen::MatrixXd matA);
    void normalizeValues(Eigen::MatrixXd& mBlock);

	template<class T>
	std::string getObjectString(T& t) {
		std::ostringstream oss;
		oss << t;

		return oss.str();
	}
};

#endif /* SRC_UTILITIES_H_ */
