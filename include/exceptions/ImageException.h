/*
 * ImageException.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_
#define INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_

#include "GenericException.h"

namespace ImageException {
	class FileSizeException : public GenericException::BaseException {
	public:
		FileSizeException() : BaseException("File size does not match") { }
		virtual ~FileSizeException() throw() { }
	};

	class NotSuppertedDepthException : public GenericException::BaseException {
	public:
		NotSuppertedDepthException() : BaseException("Not supported Depth") { }
		virtual ~NotSuppertedDepthException() throw() { }
	};
}

#endif /* INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_ */
