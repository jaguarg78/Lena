/*
 * Watermark.h
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#ifndef SRC_WATERMARK_H_
#define SRC_WATERMARK_H_

#include <cstddef>

class Watermark {
public:
	Watermark();
	virtual ~Watermark() throw();

	virtual bool isEmbeddable() = 0;
	virtual void progressInsertion() = 0;
	virtual void progressExtraction() = 0;

	int insertLogo();
	int extractLogo();
	void showProgress();

	void setWidth(int iWidth);
	void setHeight(int iHegiht);
	void setInputImageData(unsigned char *pucInputImageData);
	void setInputLogoData(unsigned char *pucInputLogoData);
	void setOutputImageData(unsigned char *pucOutputImageData);
	void setOutputLogoData(unsigned char *pucOutputLogoData);
protected:
	int				 _iWidth;
	int				 _iHeight;
	unsigned char 	*_pucInputImageData;
	unsigned char 	*_pucInputLogoData;
	unsigned char 	*_pucOutputImageData;
	unsigned char 	*_pucOutputLogoData;
};

#endif /* SRC_WATERMARK_H_ */
