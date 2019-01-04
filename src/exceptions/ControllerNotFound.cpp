/*
 * ControllerNotFound.cpp
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#include "ControllerNotFound.h"

namespace Rgb {
namespace Exceptions {

ControllerNotFound::ControllerNotFound(string portName): boost::exception() {
	this->portName = portName;
}

ControllerNotFound::~ControllerNotFound() {

}

string ControllerNotFound::getPortName() {
	return this->portName;
}

} /* namespace Exceptions */
} /* namespace Rgb */
