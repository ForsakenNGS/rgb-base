/*
 * PortException.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef EXCEPTIONS_PORTEXCEPTION_H_
#define EXCEPTIONS_PORTEXCEPTION_H_

#include <string>
#include <libserialport.h>
#include <boost/exception/exception.hpp>

using namespace std;

namespace Rgb {
namespace Exceptions {

enum PortError {
	ERR_NOT_FOUND,
	ERR_OPEN_FAILED,
	ERR_FLUSH_FAILED,
	ERR_WAIT_FAILED,
	ERR_BAUDRATE_FAILED,
	ERR_READ_FAILED,
	ERR_WRITE_FAILED,
	ERR_CLOSE_FAILED
};

class PortException: public boost::exception {
protected:
	PortError error;
	string portName;
	sp_return result;
public:
	PortException(PortError error, string portName, sp_return result);
	virtual ~PortException();

	PortError getError();
	string getPortName();
	sp_return getResult();
};

} /* namespace Exceptions */
} /* namespace Rgb */

#endif /* EXCEPTIONS_PORTEXCEPTION_H_ */
