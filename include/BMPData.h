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

    BMPData(const IImageData& rSource);
	BMPData(const std::string& sFileName);
	BMPData(const std::vector<unsigned char>& data,
	        int iWidth,
	        int iHeight,
	        const std::vector<PixelTable> *pColormap = NULL,
	        Type type = TYPE_BINARY);
	BMPData(int iWidth,
	        int iHeight,
	        const std::vector<PixelTable> *pColormap = NULL,
	        Type type = TYPE_BINARY);
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

	BitMapFileHeader          _stFileHeader;
	BitMapInfoHeader          _stInfoHeader;

	std::vector<PixelTable>  *_pColorMapData;
	Type                      _type;

private:
	BMPData();

	void initStructuresData(const std::vector<PixelTable> *pColorMap);
	void initBinaryColorMap();

	unsigned char invertByte(unsigned char byte);
};

#endif /* BMPDATA_H_ */
