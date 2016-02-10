/*
 * BMPData.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef BMPDATA_H_
#define BMPDATA_H_

#include <cmath>

#include "IImageData.h"

#include "definitions/BMPFile.h"

class BMPData: public IImageData {
public:
    enum Type {
        TYPE_INVALID,
        TYPE_BINARY,
        TYPE_RGB888
    };

	BMPData(const std::string& sFileName);
	BMPData(const std::vector<unsigned char>& data,
	        int iWidth,
	        int iHeight,
	        Type type = TYPE_BINARY,
	        const std::vector<PixelTable> *pColormap = NULL);
	BMPData(const IImageData& rSource);
	virtual ~BMPData() throw();

protected:
	virtual int getInputWidth() const;
    virtual int getInputHeight() const;
    unsigned int getInputDataSize() const;
    virtual unsigned short getBitCount() const;

	virtual void parseFileData();
	virtual void insertFileData();
	virtual void getHeaderDataStream(std::ostream& os);

	virtual void invertData();

	BitMapFileHeader    _stFileHeader;
	BitMapInfoHeader    _stInfoHeader;

	Type                _type;

	std::vector<PixelTable>  *_pColorMapData;

private:
	unsigned char invertByte(unsigned char byte);
	void initBinaryColorMap();
};

#endif /* BMPDATA_H_ */
