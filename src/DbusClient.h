/*
 * DbusClient.h
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#ifndef DBUSCLIENT_H_
#define DBUSCLIENT_H_

#include "Controller.h"
#include <iostream>
#include <dbus/dbus.h>
#include "ControllerConfig.h"

using namespace std;

namespace Rgb {

class DbusClient {
protected:
	DBusError dbusError;
	DBusConnection* dbusConnection;
	list<std::shared_ptr<ControllerInfo>> controllerList;
public:
	DbusClient(string name);
	virtual ~DbusClient();

	list<std::shared_ptr<ControllerInfo>>* getControllerList();
	bool sendConfiguration(string controllerName);
	bool sendConfigurationSingle(string controllerName, uint32_t configIndex, ControllerConfig portConfig);
	bool sendConfigurationActive(string controllerName, uint32_t portIndex, uint32_t configIndex);
	bool sendConfigurationEffect(string controllerName, uint32_t configIndex, uint32_t effectType, uint32_t effectDuration);
	bool sendConfigurationColor(string controllerName, uint32_t configIndex, uint32_t colorIndex, ControllerConfigColor color);
	bool writeToEeprom();
	void updateControllers();
	void updateConfiguration(string controllerName, std::shared_ptr<ControllerInfo> controller);
	void setConfig(string controllerName, uint32_t portIndex, uint32_t configIndex);
	void setEffect(string controllerName, uint32_t configIndex, ControllerConfigType effectType, uint32_t effectDuration);
	void setColor(string controllerName, uint32_t configIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
};

} /* namespace Rgb */

#endif /* DBUSCLIENT_H_ */
