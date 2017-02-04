/*
 * TestImageData.h
 *
 *  Created on: Feb 7, 2016
 *      Author: gianni
 */

#ifndef SRC_TESTIMAGEDATA_H_
#define SRC_TESTIMAGEDATA_H_

#include "IImageData.h"

class TestImageData: public IImageData {
public:
    TestImageData(const std::string& sFileName,
                  int iWidth,
                  int iHeight,
                  const std::vector<unsigned char>& data) : IImageData(sFileName) {
        _uiFileSize = 0;
        _uiDataSize = data.size();
        _iWidth = iWidth;
        _iHeight = iHeight;
        _pcData = const_cast<unsigned char *>(data.data());
        _isOutputFile = true;
    }
    virtual ~TestImageData() throw() { }
protected:
    virtual int getInputWidth() const {
        return _iWidth;
    }
    virtual int getInputHeight() const {
        return _iHeight;
    }
    virtual unsigned int getInputDataSize() const {
        return _uiDataSize;
    }
    virtual unsigned short getBitCount() const {
        return 3;
    }

    virtual void parseFileData() {

    }
    virtual void insertFileData() {

    }
    virtual void getHeaderDataStream(std::ostream& os)  {

    }

};

#endif /* SRC_TESTIMAGEDATA_H_ */
