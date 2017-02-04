/*
 * FactoryType.h
 *
 *  Created on: Jan 23, 2016
 *      Author: gianni
 */

#ifndef SRC_FACTORYTYPE_H_
#define SRC_FACTORYTYPE_H_

#include "Watermark.h"

class FactoryType {
public:
	static Watermark* create(int iType);
private:
	FactoryType();
	virtual ~FactoryType();

	FactoryType(const FactoryType& source);
	FactoryType& operator=(const FactoryType& source);
};

#endif /* SRC_FACTORYTYPE_H_ */
