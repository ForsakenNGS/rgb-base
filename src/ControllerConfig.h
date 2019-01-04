/*
 * ControllerConfig.h
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#ifndef CONTROLLERCONFIG_H_
#define CONTROLLERCONFIG_H_

#include "PortConfig.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>

using namespace std;
using namespace boost;

namespace Rgb {

class ControllerConfig {
protected:
	string portName;
	struct ControllerVersion {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	} version;
	map<uint32_t,PortConfig> controllerPorts;
public:
	ControllerConfig(string portName);
	virtual ~ControllerConfig();

	string getPortName();
	map<uint32_t,PortConfig>* getPortList();
	int getVerionMajor();
	int getVerionMinor();
	int getVerionPatch();
	string getVersionString();
	void setEffect(uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration);
	void setColor(uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
	void setVersion(uint32_t major, uint32_t minor, uint32_t patch);
};

} /* namespace Rgb */

#endif /* CONTROLLERCONFIG_H_ */
