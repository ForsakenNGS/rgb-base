/*
 * PortListFailed.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef EXCEPTIONS_PORTLISTFAILED_H_
#define EXCEPTIONS_PORTLISTFAILED_H_

#include "PortException.h"

namespace Rgb {
namespace Exceptions {

class PortListFailed: public boost::exception {
public:
	PortListFailed(sp_return result);
	virtual ~PortListFailed();
};

} /* namespace Exceptions */
} /* namespace Rgb */

#endif /* EXCEPTIONS_PORTLISTFAILED_H_ */
