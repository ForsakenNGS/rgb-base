/*
 * Controller.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <cstdint>
#include <iterator>
#include <iostream>
#include <map>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/thread/thread.hpp>
#include <boost/timer/timer.hpp>

#include "ControllerInfo.h"
#include "Interface.h"
#include "exceptions/PortException.h"
#include "exceptions/ControllerNotFound.h"

using namespace std;
using namespace boost;

namespace Rgb {

class Controller : public ControllerInfo, public Interface {
protected:
	sp_port* portHandle;
	sp_event_set* portWaitEvent;
	bool portOpen;
	stringstream readStream;

	void freeHandle();
	void outputText(string text);
	void outputVersion();
	void outputBuffer();
public:
	Controller(string portName);
	virtual ~Controller();

	void close();
	void open();
	void update();
	void receivedReply(string command);
	void writeCommand(string command);

	void setConfig(uint32_t portIndex, uint32_t configIndex);
	void setEffect(int configIndex, ControllerConfigType effectType, int effectDuration);
	void setColor(int configIndex, int colorIndex, uint8_t red, uint8_t green, uint8_t blue);
private:
	regex welcomeCheck;
	regex portConfig;
	regex configEffect;
	regex configColor;
};

} /* namespace Rgb */

#endif /* CONTROLLER_H_ */
