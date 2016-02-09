/*
 * ImageException.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_
#define INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_

#include <exception>

namespace ImageException {
	class BaseException : public std::exception {
	public:
		BaseException() : _sMessage("General Error") { }
		BaseException(const std::string& sMessage) : _sMessage(sMessage) { }
		virtual ~BaseException() throw() { }

		virtual const char *what() const throw() {
			return _sMessage.c_str();
		}
	private:
		std::string		_sMessage;
	};

	class FileSizeException : public BaseException {
	public:
		FileSizeException() : BaseException("File size does not match") { }
		virtual ~FileSizeException() throw() { }
	};

	class NotSuppertedDepthException : public BaseException {
	public:
		NotSuppertedDepthException() : BaseException("Not supported Depth") { }
		virtual ~NotSuppertedDepthException() throw() { }
	};
}

#endif /* INCLUDE_EXCEPTIONS_IMAGEEXCEPTION_H_ */
