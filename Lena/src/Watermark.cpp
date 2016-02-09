/*
 * Watermark.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: gianni
 */

#include "Watermark.h"

Watermark::Watermark() : _iWidth(),
						 _iHeight(),
						 _pucInputImageData(NULL),
						 _pucInputLogoData(NULL),
						 _pucOutputImageData(NULL),
						 _pucOutputLogoData(NULL) {
	// TODO Auto-generat_ed constructor stub

}

Watermark::~Watermark() throw() {
	if (_pucInputImageData) {
		delete[] _pucInputImageData;
		_pucInputImageData = NULL;
	}

	if (_pucInputLogoData) {
		delete[] _pucInputLogoData;
		_pucInputLogoData = NULL;
	}

	if (_pucOutputImageData) {
		delete[] _pucOutputImageData;
		_pucOutputImageData = NULL;
	}

	if (_pucOutputLogoData) {
		delete[] _pucOutputLogoData;
		_pucOutputLogoData = NULL;
	}
}

int Watermark::insertLogo() {
	return 0;
}

int Watermark::extractLogo() {
	return 0;
}

void Watermark::showProgress() {

}

void Watermark::setWidth(int iWidth) {
	_iWidth = iWidth;
}

void Watermark::setHeight(int iHeight) {
	_iHeight = iHeight;
}

void Watermark::setInputImageData(unsigned char *pucInputImageData) {
	_pucInputImageData = pucInputImageData;
}

void Watermark::setInputLogoData(unsigned char *pucInputLogoData) {
	_pucInputLogoData = pucInputLogoData;
}

void Watermark::setOutputImageData(unsigned char *pucOutputImageData) {
	_pucOutputImageData = pucOutputImageData;
}

void Watermark::setOutputLogoData(unsigned char *pucOutputLogoData) {
	_pucOutputLogoData = pucOutputLogoData;
}
