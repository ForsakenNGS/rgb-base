/*
 * PortException.cpp
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#include "PortException.h"

namespace Rgb {
namespace Exceptions {

PortException::PortException(PortError error, string portName, sp_return result): boost::exception() {
	this->error = error;
	this->portName = portName;
	this->result = result;
}

PortException::~PortException() {
	// TODO Auto-generated destructor stub
}

PortError PortException::getError() {
	return this->error;
}

string PortException::getPortName() {
	return this->portName;
}

sp_return PortException::getResult() {
	return this->result;
}

} /* namespace Exceptions */
} /* namespace Rgb */
