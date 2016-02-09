/*
 * IImageData.hs
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef IIMAGEDATA_H_
#define IIMAGEDATA_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "exceptions/ImageException.h"

class IImageData {
public:
    IImageData(const std::string& sFileName);
    IImageData(const std::vector<unsigned char>& data, int iWidth, int iHeight);
    virtual ~IImageData() throw ();

    std::string getFileName() const;
    const unsigned char *getData() const;

    int getWidth() const;
    int getHeight() const;
    unsigned int getDataSize() const;
    unsigned int getRowSizeFixed() const;

    virtual void init();
    virtual void create(const std::string& sFileName,
                        bool invert = false);

    friend std::ostream& operator<<(std::ostream& os, IImageData& data) {
        os << "*********************************" << std::endl;
        os << "File Name  :" << data.getFileName() << std::endl;
        data.getHeaderDataStream(os);
        return os;
    }

protected:
    virtual int getInputWidth() const = 0;
    virtual int getInputHeight() const = 0;
    virtual unsigned int getInputDataSize() const = 0;
    virtual unsigned short getBitCount() const = 0;

    virtual void parseFileData() = 0;
    virtual void insertFileData() = 0;
    virtual void getHeaderDataStream(std::ostream& os) = 0;

    virtual void invertData();

    std::string     _sFileName;
    unsigned int    _uiFileSize;
    unsigned int    _uiDataSize;
    int             _iWidth;
    int             _iHeight;
    unsigned char  *_pcData;

    std::ifstream   _ifImageFile;
    std::ofstream   _ofImageFile;

    bool _isOutputFile;
private:

};

#endif /* IIMAGEDATA_H_ */
