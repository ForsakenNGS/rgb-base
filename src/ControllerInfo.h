/*
 * ControllerConfig.h
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#ifndef CONTROLLERINFO_H_
#define CONTROLLERINFO_H_

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include "ControllerConfig.h"

using namespace std;
using namespace boost;

namespace Rgb {

class ControllerInfo {
protected:
	string portName;
	struct ControllerVersion {
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	} version;
	map<uint32_t,uint32_t> controllerPort;
	map<uint32_t,ControllerConfig> controllerConfig;
public:
	ControllerInfo(string portName);
	virtual ~ControllerInfo();

	string getPortName();
	map<uint32_t,uint32_t>* getPortList();
	map<uint32_t,ControllerConfig>* getConfigList();
	int getVerionMajor();
	int getVerionMinor();
	int getVerionPatch();
	string getVersionString();
	void setConfig(uint32_t portIndex, uint32_t configIndex);
	void setEffect(uint32_t configIndex, ControllerConfigType effectType, uint32_t effectDuration);
	void setColor(uint32_t configIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
	void setVersion(uint32_t major, uint32_t minor, uint32_t patch);
};

} /* namespace Rgb */

#endif /* CONTROLLERINFO_H_ */
