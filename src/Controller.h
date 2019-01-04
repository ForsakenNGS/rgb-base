/*
 * Controller.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <iterator>
#include <iostream>
#include <map>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/thread/thread.hpp>
#include <boost/timer/timer.hpp>

#include "Interface.h"
#include "ControllerConfig.h"
#include "exceptions/PortException.h"
#include "exceptions/ControllerNotFound.h"

using namespace std;
using namespace boost;

namespace Rgb {

class Controller : public ControllerConfig, public Interface {
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

	void setEffect(int portIndex, PortConfigType effectType, int effectDuration);
	void setColor(int portIndex, int colorIndex, uint8_t red, uint8_t green, uint8_t blue);
private:
	regex welcomeCheck;
	regex portEffect;
	regex portColor;
};

} /* namespace Rgb */

#endif /* CONTROLLER_H_ */
