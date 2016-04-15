/*
 * BMPData.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#include "BMPData.h"

BMPData::BMPData(const std::string& sFileName) : IImageData(sFileName),
                                                 _pColorMapData(NULL),
                                                 _type(TYPE_INVALID) {
	std::memset(&_stFileHeader, 0, sizeof(BitMapFileHeader));
	std::memset(&_stInfoHeader, 0, sizeof(BitMapInfoHeader));
}

BMPData::BMPData(const IImageData& rSource) : IImageData(rSource),
                                              _pColorMapData(NULL),
                                              _type(TYPE_INVALID) {

}

BMPData::BMPData(const std::vector<unsigned char>& data,
                 int iWidth,
                 int iHeight,
                 const std::vector<PixelTable> *pColorMap,
                 Type type) : IImageData(data, iWidth, iHeight),
                              _pColorMapData(NULL),
                              _type(type) {

    initStructuresData(pColorMap);
}

BMPData::BMPData(int iWidth,
                 int iHeight,
                 const std::vector<PixelTable> *pColorMap,
                 Type type) : IImageData(iWidth, iHeight),
                              _pColorMapData(NULL),
                              _type(type) {

    /*
     * Warning!!! Data size must be set in this point because of in order to
     * calculate the RowSizeFixed specific calculations of this
     * class is needed.
     */
    _uiDataSize = getRowSizeFixed() * _iHeight;
    _pcData = new unsigned char[_uiDataSize];
    std::memset(_pcData, 0, _uiDataSize);

    initStructuresData(pColorMap);

    // DEBUG
    std::cout << "New Data Size: " << _uiDataSize << std::endl;
}

BMPData::~BMPData() throw() {
    if (_pColorMapData && !_isOutputFile) {
        delete _pColorMapData;
        _pColorMapData = NULL;
    }
}

void BMPData::initStructuresData(const std::vector<PixelTable> *pColorMap) {
    if (pColorMap) {
        _pColorMapData = const_cast<std::vector<PixelTable> *>(pColorMap);
    } else if (_type == TYPE_BINARY) {
        initBinaryColorMap();
    }

    // DEBUG
//    if (pColorMap) {
//        _pColorMapData = pColorMap; //new std::vector<PixelTable>(*pColorMap);
//        std::cout << " ********** Color 1 *******" << std::endl;
//        for (unsigned int i = 0; i < pColorMap->size(); i++) {
//            std::cout << std::hex << (int) (*pColorMap)[i].pixel.byteRed << ", "
//                                  << (int) (*pColorMap)[i].pixel.byteGreen << ", "
//                                  << (int) (*pColorMap)[i].pixel.byteBlue << ", "
//                                  << (int) (*pColorMap)[i].byteUnused
//                                  << std::endl;
//        }
//        std::cout << " ********** Color 2 *******" << std::endl;
//        for (unsigned int i = 0; i < _pColorMapData->size(); i++) {
//            std::cout << std::hex << (int) (*_pColorMapData)[i].pixel.byteRed << ", "
//                                  << (int) (*_pColorMapData)[i].pixel.byteGreen << ", "
//                                  << (int) (*_pColorMapData)[i].pixel.byteBlue << ", "
//                                  << (int) (*_pColorMapData)[i].byteUnused
//                                  << std::endl;
//        }
//        std::cout << std::dec;
//    }

    _stFileHeader.bfType = 0x0042 | (0x004D << 8); // BM Signature
    _stFileHeader.bfReserved1 = 0;
    _stFileHeader.bfReserved2 = 0;

    _stInfoHeader.biSize = sizeof(BitMapInfoHeader);
    _stInfoHeader.Width = _iWidth;
    _stInfoHeader.Height = _iHeight;
    _stInfoHeader.biPlanes = 1;
    _stInfoHeader.biBitCount = getBitCount();
    _stInfoHeader.biCompresssion = 0;
    _stInfoHeader.biSizeImage = getRowSizeFixed() * std::abs(_iHeight);
    _stInfoHeader.biXPelsPerMeter = std::ceil(72. * (100. / 2.54));
    _stInfoHeader.biYPelsPerMeter = std::ceil(72. * (100. / 2.54));
    _stInfoHeader.biClrUsed = (_pColorMapData ? _pColorMapData->size() : 0);
    _stInfoHeader.biClrImportant = (_pColorMapData ? _pColorMapData->size(): 0);

    _stFileHeader.bfOffBits = sizeof(BitMapFileHeader) +
                              _stInfoHeader.biSize +
                              (_pColorMapData ?
                                      (_pColorMapData->size() * sizeof(PixelTable)) : 0);
    _stFileHeader.bfSize = _stFileHeader.bfOffBits + _stInfoHeader.biSizeImage;
}

unsigned short BMPData::getBitCount() const {
    // DEBUG
//    std::cout << "getBitCount" << std::endl;
//    std::cout << _type << std::endl;
    switch (_type) {
    case TYPE_BINARY:
        return BIT_COUNT_BINARY_IMAGE;
    case TYPE_RGB888:
        return BIT_COUNT_RGB_IMAGE;
    default:
       throw;
    }
}

void BMPData::parseFileData() {
	// Get BMP file header
	_ifImageFile.seekg(0, std::ios::beg);
	_ifImageFile.read((char *) &_stFileHeader, sizeof(BitMapFileHeader));

	// Verify the file size
	if (_uiFileSize != _stFileHeader.bfSize) {
		throw ImageException::FileSizeException();
	}

	// Get BMP image info header
	_ifImageFile.seekg(sizeof(BitMapFileHeader), std::ios::beg);
	_ifImageFile.read((char *) &_stInfoHeader, sizeof(BitMapInfoHeader));

	std::cout << "*********" << _stInfoHeader.biBitCount << std::endl;
	// Verify image bit count
	switch (_stInfoHeader.biBitCount) {
	case BIT_COUNT_BINARY_IMAGE:
	    _type = TYPE_BINARY;
	    break;
	case BIT_COUNT_RGB_IMAGE:
	    _type = TYPE_RGB888;
	    break;
	default:
	    throw ImageException::NotSuppertedDepthException();
	}

	// Fill Additional information
	unsigned int uiMapSize = (_stFileHeader.bfOffBits - (sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader))) / sizeof(PixelTable);
    if (uiMapSize > 0) {
        _pColorMapData = new std::vector<PixelTable>(uiMapSize);
        _ifImageFile.seekg(sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader), std::ios::beg);
        _ifImageFile.read((char *) _pColorMapData->data(), (uiMapSize * sizeof(PixelTable)));
    }

	// Fill data array.
	_pcData = new unsigned char[_stInfoHeader.biSizeImage];
	_ifImageFile.seekg(_stFileHeader.bfOffBits, std::ios::beg);
	_ifImageFile.read((char *) _pcData, _stInfoHeader.biSizeImage);
}

void BMPData::insertFileData() {

    _ofImageFile.write((char *) &_stFileHeader, sizeof(BitMapFileHeader));
    _ofImageFile.write((char *) &_stInfoHeader, sizeof(BitMapInfoHeader));
    if (_pColorMapData && !_pColorMapData->empty()) {
        _ofImageFile.write((char *) _pColorMapData->data(), _pColorMapData->size() * sizeof(PixelTable));
    }
    _ofImageFile.write((char *) _pcData, _stInfoHeader.biSizeImage);

    // DEBUG
//    std::cout << "****************** Test *******************" << std::endl;
//    for (unsigned int  i = 0; i < _stInfoHeader.biSizeImage; i++) {
//        if ((i * 8) % _iWidth == 0) {
//            std::cout << std::endl << "[" << std::dec << i << "]";
//        }
//
//        std::cout << std::hex << (int) _pcData[i] << " ";
//    }
//
//    std::cout << std:: dec << std::endl;
}

void BMPData::invertData() {
    switch (_type) {
    case TYPE_BINARY:
        for (unsigned int i = 0; i < (_stInfoHeader.biSizeImage / 2); i++) {
            unsigned char byteData = invertByte(_pcData[i]);
            unsigned char byteDataTemp = invertByte(_pcData[_stInfoHeader.biSizeImage - (i + 1)]);

            _pcData[_stInfoHeader.biSizeImage - (i + 1)] = byteData;
            _pcData[i] = byteDataTemp;
         }
        break;
    case TYPE_RGB888: {
            unsigned int pixelAmountData = _stInfoHeader.biSizeImage / sizeof(Pixel);
            for (unsigned int i = 0; i < (pixelAmountData / 2); i++) {
                Pixel tempPixel;
                std::memcpy(&tempPixel, _pcData + (i * sizeof(Pixel)), sizeof(Pixel));
                std::memcpy(_pcData + (i * sizeof(Pixel)),
                            _pcData + _stInfoHeader.biSizeImage - ((i + 1) * sizeof(Pixel)),
                            sizeof(Pixel));
                std::memcpy(_pcData + _stInfoHeader.biSizeImage - ((i + 1) * sizeof(Pixel)),
                            &tempPixel,
                            sizeof(Pixel));
            }
        }
        break;
    case TYPE_INVALID:
    default:
        break;
    }
}

unsigned char BMPData::invertByte(unsigned char ucByte) {
    unsigned char ucTempByte = 0;
    for (int i = 0; i < CHAR_BIT; i++) {
        ucTempByte |= (ucByte & 0x80) >> (CHAR_BIT - (i + 1));
        ucByte <<= 1;
    }

    return ucTempByte;
}

void BMPData::initBinaryColorMap() {
    PixelTable pixelMap[2];
    pixelMap[0].pixel.byteRed = 0x00;
    pixelMap[0].pixel.byteGreen = 0x00;
    pixelMap[0].pixel.byteBlue = 0x00;
    pixelMap[0].byteUnused = 0x00;

    pixelMap[1].pixel.byteRed = 0xFF;
    pixelMap[1].pixel.byteGreen = 0xFF;
    pixelMap[1].pixel.byteBlue = 0xFF;
    pixelMap[1].byteUnused = 0x00;

    _pColorMapData = new std::vector<PixelTable>(
                        pixelMap,
                        pixelMap + (sizeof(pixelMap) / sizeof(PixelTable)));
}

int BMPData::getInputWidth() const {
    return _stInfoHeader.Width;
}

int BMPData::getInputHeight() const {
    return _stInfoHeader.Height;
}

unsigned int BMPData::getInputDataSize() const {
    return _stInfoHeader.biSizeImage;
}

void BMPData::getHeaderDataStream(std::ostream& os) {
    os << "*** File Header ***" << std::endl
       << "Magic Id   : "
           << (char) (_stFileHeader.bfType & 0x00FF)
           << (char) ((_stFileHeader.bfType & 0xFF00) >> 8) << std::endl
       << "File Size  : " << _stFileHeader.bfSize << std::endl
       << "Reserved1  : " << _stFileHeader.bfReserved1 << std::endl
       << "Reserved2  : " << _stFileHeader.bfReserved2 << std::endl
       << "Offset     : " << _stFileHeader.bfOffBits << std::endl;

    os << "*** Info Header ***" << std::endl
       << "Info Size  : " << _stInfoHeader.biSize << std::endl
       << "Img Width  : " << _stInfoHeader.Width << std::endl
       << "Img Height : " << _stInfoHeader.Height << std::endl
       << "Planes     : " << _stInfoHeader.biPlanes << std::endl
       << "BitCount   : " << _stInfoHeader.biBitCount << std::endl
       << "Compression: " << _stInfoHeader.biCompresssion << std::endl
       << "Image Size : " << _stInfoHeader.biSizeImage << std::endl
       << "XPelsPerMet: " << _stInfoHeader.biXPelsPerMeter << std::endl
       << "YPelsPerMet: " << _stInfoHeader.biYPelsPerMeter << std::endl
       << "Used Color : " << _stInfoHeader.biClrUsed << std::endl
       << "Imp Color  : " << _stInfoHeader.biClrImportant << std::endl;

    if (_pColorMapData) {
        os << "*** Color Map *** " << std::endl;
        for (unsigned int i = 0; i < _pColorMapData->size(); i++) {
            os << "[" << std::hex << std::setw(3) << (int) (*_pColorMapData)[i].pixel.byteRed << " "
                                  << std::setw(3) << (int) (*_pColorMapData)[i].pixel.byteBlue << " "
                                  << std::setw(3) << (int) (*_pColorMapData)[i].pixel.byteGreen << "]" << std::dec << std::endl;

        }
    }
}
