/*
 * IImageData.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#include "IImageData.h"

IImageData::IImageData(const std::string& sFileName) : _sFileName(sFileName),
													   _uiFileSize(0),
													   _uiDataSize(0),
                                                       _iWidth(0),
                                                       _iHeight(0),
                                                       _pcData(NULL),
                                                       _isOutputFile(false),
                                                       _hasOwnData(true) {

}

IImageData::IImageData(const std::vector<unsigned char>& data,
                       int iWidth,
                       int iHeight) : _sFileName(""),
                                      _uiFileSize(0),
                                      _uiDataSize(data.size()),
                                      _iWidth(iWidth),
                                      _iHeight(iHeight),
                                      _pcData(const_cast<unsigned char *>(data.data())),
                                      _isOutputFile(true),
                                      _hasOwnData(false) {

}

IImageData::IImageData(const IImageData& rSource) : _sFileName("out." + rSource.getFileName()),
                                                    _uiFileSize(0),
                                                    _uiDataSize(rSource.getDataSize()),
                                                    _iWidth(rSource.getWidth()),
                                                    _iHeight(rSource.getHeight()),
                                                    _pcData(NULL),
                                                    _isOutputFile(true),
                                                    _hasOwnData(true) {
    _pcData = new unsigned char[rSource.getDataSize()];
    std::memcpy(_pcData, rSource.getData(), rSource.getDataSize());
}

IImageData::~IImageData() throw() {
	if (_pcData && _hasOwnData) {
		delete [] _pcData;
		_pcData = NULL;
	}

	if (_ifImageFile.is_open()) {
		_ifImageFile.close();
	}

	if (_ofImageFile.is_open()) {
        _ofImageFile.close();
    }
}

void IImageData::init() {
    std::cout << "Init input_file" << std::endl;

    if (!_isOutputFile) {
        _ifImageFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            _ifImageFile.open(
                    _sFileName.c_str(),
                    std::ios::in | std::ios::binary | std::ios::ate);
            if (_ifImageFile.is_open()) {
                // Get File size
                _uiFileSize = _ifImageFile.tellg();

                // Parse headers depending on the file type and
                // initiate the data array
                parseFileData();
            }
        } catch (std::ifstream::failure& e) {
            throw ImageException::BaseException();
        }

        _ifImageFile.close();
    } else {
        // Throw exception bad procedure
    }
}

void IImageData::create(const std::string& sFileName,
                        bool invert) {
    std::cout << "Creating output_file" << std::endl;

    if (invert) {
        invertData();
    }

    if (_isOutputFile) {
        _ofImageFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            _ofImageFile.open(
                    sFileName.c_str(),
                    std::ios::out | std::ios::binary);

            if (_ofImageFile.is_open()) {

                // File data insertion
                insertFileData();
            }
        } catch (std::ofstream::failure& e) {
            throw ImageException::BaseException();
        }

        _ofImageFile.close();
    } else {
        // Throw exception bad procedure
    }
}

void IImageData::invertData() {

}

std::string IImageData::getFileName() const {
	return _sFileName;
}

int IImageData::getWidth() const {
    if (_isOutputFile) {
        return _iWidth;
    }

    return getInputWidth();
}

int IImageData::getHeight() const {
    if (_isOutputFile) {
        return _iHeight;
    }

    return getInputHeight();
}

unsigned int IImageData::getDataSize() const {
    if (_isOutputFile) {
        if (getRowSizeFixed() * getWidth() != _uiDataSize) {
            // Throw exception data size wrong
            std::cout << "Data size wrong" << std::endl;
        }
        return _uiDataSize;
    }

    return getInputDataSize();
}

unsigned int IImageData::getRowSizeFixed() const {
//    std::cout << "getRowSizeFixed" << std::endl;
//    std::cout << getBitCount() << std::endl;
//    std::cout << getWidth() << std::endl;
    // floor function is not necessary because this is an operation between ints
    return ((getBitCount() * getWidth() + 31) / 32) * 4;
}

unsigned char *IImageData::getData() const {
    return _pcData;
}

const unsigned char *IImageData::getConstData() const {
    return _pcData;
}
