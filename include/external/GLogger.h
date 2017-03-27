/*
 * GLogger.h
 *
 *  Created on: Feb 27, 2017
 *      Author: gianni
 */

#ifndef GLOGGER_H_
#define GLOGGER_H_

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

class GLogger {
public:
	typedef enum ePriorityLevel {
		EMERG  = log4cpp::Priority::EMERG,
		FATAL  = log4cpp::Priority::FATAL,
		ALERT  = log4cpp::Priority::ALERT,
		CRIT   = log4cpp::Priority::CRIT,
		ERROR  = log4cpp::Priority::ERROR,
		WARN   = log4cpp::Priority::WARN,
		NOTICE = log4cpp::Priority::NOTICE,
		INFO   = log4cpp::Priority::INFO,
		DEBUG  = log4cpp::Priority::DEBUG,
		NOTSET = log4cpp::Priority::NOTSET
	} Priority;

	static GLogger *getInstance();
	static void removeInstance();
	static void init(const std::string& sPropertiesFilePath);
	static void log(GLogger::Priority  priority,
			        const std::string& sMessage);
private:
	GLogger();
	virtual ~GLogger() throw();

	log4cpp::Category& getCategory();

	static GLogger 	   *_pInstance;
	log4cpp::Category& 	_rLogRoot;
};


#endif /* GLOGGER_H_ */
