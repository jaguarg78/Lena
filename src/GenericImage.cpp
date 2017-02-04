/*
 * GenericImageData.cpp
 *
 *  Created on: Jan 26, 2016
 *      Author: gianni
 */

#include <GenericImage.h>

GenericImage::GenericImage(const std::string& sFileName) : IImageData(sFileName) {
    _pimImage = new QImage(sFileName.c_str());
}

GenericImage::~GenericImage() throw () {
    if (_pimImage) {
        delete _pimImage;
        _pimImage = NULL;
    }
}

void GenericImage::init() {
    // Nothing to do
}

void GenericImage::create(const std::string& sFileName,
                          bool invert) {
    _pimImage = new QImage(sFileName.c_str());
}

std::string GenericImage::getFileName() const {
    return _sFileName;
}

int GenericImage::getInputWidth() const {
    return _pimImage->width();
}

int GenericImage::getInputHeight() const {
    return _pimImage->height();
}

unsigned int GenericImage::getInputDataSize() const {
    return _pimImage->byteCount();
}

unsigned short GenericImage::getBitCount() const {
    return _pimImage->bitPlaneCount();
}

bool GenericImage::isBinaryImage() const {
    return _pimImage->format() == QImage::Format_Mono;
}

unsigned int GenericImage::getData(std::vector<unsigned char>& rImageData) const {
    unsigned int uiCounter = 0;

    const unsigned char *pTempData = _pimImage->constBits();
    unsigned char *pImageData = rImageData.data();

    switch (_pimImage->format()) {
    // Be careful because the returned data is not only backwards but it has also
    // the bit value inverted.
    case QImage::Format_Mono:
        // Avoid trying to store info out of bounds of vector
        if ((unsigned int) _pimImage->byteCount() != rImageData.size()) {
            // Throw exception
        }

        std::memcpy(pImageData,
                    pTempData,
                    (unsigned int) _pimImage->byteCount());

        uiCounter = _pimImage->byteCount();
        break;
    case QImage::Format_RGB888:
        // Avoid trying to store info out of bounds of vector
        if ((_pimImage->width() * _pimImage->height() * sizeof(Pixel)) != rImageData.size()) {
            // Throw exception
        }

        for (int i = 0; i < (_pimImage->height() * _pimImage->width()); i++) {
            QRgb tempPixel;
            unsigned int uiTempOffset = i * sizeof(QRgb);
            std::memcpy((unsigned char *) &tempPixel,
                        pTempData + uiTempOffset,
                        sizeof(QRgb));

            Pixel outPixel;
            outPixel.byteRed = qRed(tempPixel);
            outPixel.byteGreen = qGreen(tempPixel);
            outPixel.byteBlue = qBlue(tempPixel);

            unsigned int uiOffset = (i * sizeof(Pixel));
            std::memcpy(pImageData + uiOffset,
                        (unsigned char *) &outPixel,
                        sizeof(Pixel));

            uiCounter++;
        }
        break;
    default:
        // Throw exception Not supported
        break;
    }


    return uiCounter;
}

void GenericImage::createRGB888Image(const std::string& sFileName,
                                     int iWidth,
                                     int iHeight,
                                     unsigned char *pucData,
                                     const char* pcFormat,
                                     int iQuality) {
    if (!sFileName.empty() &&
            iWidth > 0 &&
            iHeight > 0 &&
            pucData) {
        QImage output(pucData, iWidth, iHeight, QImage::Format_RGB888);
        output.save(sFileName.c_str(), pcFormat, iQuality);
    } else {
        // Throw exception
    }
}

void GenericImage::parseFileData() {

}

void GenericImage::insertFileData() {

}

void GenericImage::getHeaderDataStream(std::ostream& os) {
    if (_pimImage) {
        os << "Img Width  : " << _pimImage->width() << std::endl
           << "Img Height : " << _pimImage->height() << std::endl
           << "Image Size : " << _pimImage->byteCount() << std::endl
           << "Planes     : " << _pimImage->bitPlaneCount() << std::endl
           << "BitCount   : " << _pimImage->depth() << std::endl
           << "Format     : " << _pimImage->format() << std::endl;
    }
}

