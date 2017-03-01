/*
 * GLoggerException.h
 *
 *  Created on: Feb 27, 2017
 *      Author: gianni
 */

#ifndef GLOGGEREXCEPTION_H_
#define GLOGGEREXCEPTION_H_

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

class GLoggerPropertiesFile : public BaseException {
public:
	GLoggerPropertiesFile() : BaseException("Missing necessary parameters") { }
	virtual ~GLoggerPropertiesFile() throw() { }
};

class GLoggerNotInstatiedYet : public BaseException {
public:
	GLoggerNotInstatiedYet() : BaseException("GLogger instance is not available yet.") { }
	virtual ~GLoggerNotInstatiedYet() throw() { }
};


#endif /* GLOGGEREXCEPTION_H_ */
