/*
 * GenericException.h
 *
 *  Created on: Feb 4, 2017
 *      Author: gianni
 */

#ifndef GENERICEXCEPTION_H_
#define GENERICEXCEPTION_H_

#include <exception>

namespace GenericException {
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

	class MissingParameter : public BaseException {
	public:
		MissingParameter() : BaseException("Missing necessary parameters") { }
		virtual ~MissingParameter() throw() { }
	};
}

#endif /* GENERICEXCEPTION_H_ */
