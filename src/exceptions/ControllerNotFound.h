/*
 * ControllerNotFound.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef EXCEPTIONS_CONTROLLERNOTFOUND_H_
#define EXCEPTIONS_CONTROLLERNOTFOUND_H_

#include <string>
#include <boost/exception/exception.hpp>

using namespace std;

namespace Rgb {
namespace Exceptions {

class ControllerNotFound: public boost::exception {
protected:
	string portName;
public:
	ControllerNotFound(string portName);
	virtual ~ControllerNotFound();

	string getPortName();
};

} /* namespace Exceptions */
} /* namespace Rgb */

#endif /* EXCEPTIONS_CONTROLLERNOTFOUND_H_ */
