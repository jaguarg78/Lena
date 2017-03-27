/*
 * WMProcess.h
 *
 *  Created on: Feb 19, 2016
 *      Author: gianni
 */

#ifndef SRC_WMPROCESS_H_
#define SRC_WMPROCESS_H_

#include "Watermark.h"

namespace SProcess {
    class WMProcess_17 : public Watermark {
    public:
        enum Type_17 {
            TYPE_17_1, // From Paper 1 - 17
            TYPE_17_2  // From paper 1 - 01
        };

        WMProcess_17(int iDelta,
                     Type_17 type = TYPE_17_2,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT_S,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_17() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                    const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    private:
        WMProcess_17();

        int         _iDelta;
        Type_17     _type;
    }; /* WMProcess_17 */
//#if 0
    class WMProcess_35 : public Watermark {
    public:
        WMProcess_35(int iDelta,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT_S,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_35() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                            const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    private:
        WMProcess_35();

        int     _iDelta;
    }; /* WMProcess_35 */
//#endif
//#if 0
    class WMProcess_74 : public Watermark {
    public:
        WMProcess_74(float fModule,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT_S,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_74() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                    const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    private:
        WMProcess_74();

        float   _fQModule;
    }; /* WMProcess_74 */
//#endif
} /* namespace SProcess */

namespace UProcess {
//#if 0
    class WMProcess_43 : public Watermark {
    public:
        WMProcess_43(unsigned int uiColumns,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT_43,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_43() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                    const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    private:
        WMProcess_43();

        unsigned int    _uiColumns;
    }; /* WMProcess_43 */
//#endif
//#if 0
    class WMProcess_09 : public Watermark {
    public:
        WMProcess_09(float fThreshold,
                     unsigned int uiDataPerBlock = DATA_SIZE_PER_BLOCK_DEFAULT,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_09() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                    const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    protected:
        float   _fThreshold;

    private:
        WMProcess_09();


    }; /* WMProcess_09 */
//#endif
} /* namespace UProcess */

namespace UVProcess {
//#if 0
    class WMProcess_18 : public UProcess::WMProcess_09 {
    public:
        WMProcess_18(float fThreshold,
                     unsigned int uiDimension = BLOCK_DIMENSION_DEFAULT,
                     unsigned int uiRedundancy = REDUDANCY_DEFAULT,
                     unsigned int uiIterations = ITERATIONS_DEFAULT,
                     Coordinates  tCoordinates = TYPE_ADJACENT);
        virtual ~WMProcess_18() throw();

        virtual bool isEmbeddable(const IImageData& hostImage,
                                  const IImageData& logoImage);
        virtual bool canBeExtracted(const IImageData& logoOutputImage,
                                    const IImageData& watermarkedImage);

        virtual Eigen::MatrixXd getWMedBlock(const tSVD& rSVD,
                                             const std::vector<unsigned char>& vecData);

        virtual void extractData(const Lena::SVD& rSVD,
                                 BlockData&  stBlockData);
    private:
        WMProcess_18();
    }; /* WMProcess_18 */
//#endif
} /* namespace UVProcess */

#endif /* SRC_WMPROCESS_H_ */
