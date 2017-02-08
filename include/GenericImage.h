/*
 * GenericImageData.h
 *
 *  Created on: Jan 26, 2016
 *      Author: gianni
 */

#ifndef SRC_GENERICIMAGE_H_
#define SRC_GENERICIMAGE_H_

#include <cstring>
#include <climits>
#include <vector>

#include <QtGui/QImage>

#include "IImageData.h"
#include "definitions/Global.h"

class GenericImage : public IImageData {
public:
    GenericImage(const std::string& sFileName);
    virtual ~GenericImage() throw ();

    virtual void init();
    virtual void create(const std::string& sFileName,
                        bool invert = false);

    std::string getFileName() const;

    bool isBinaryImage() const;
    unsigned int getData(std::vector<unsigned char>& rImageData) const;

    static void createRGB888Image(const std::string& sFileName,
                                  int iWidth,
                                  int iHeight,
                                  unsigned char *pucData,
                                  const char* pcFormat = 0,
                                  int iQuality = -1);

protected:
    virtual int getInputWidth() const;
    virtual int getInputHeight() const;
    unsigned int getInputDataSize() const;
    virtual unsigned short getBitCount() const;

    virtual void parseFileData();
    virtual void insertFileData();
    virtual void getHeaderDataStream(std::ostream& os);

    std::string     _sFileName;
    QImage         *_pimImage;
};

#endif /* SRC_GENERICIMAGE_H_ */
