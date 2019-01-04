/*
 * DbusClient.h
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#ifndef DBUSCLIENT_H_
#define DBUSCLIENT_H_

#include "Controller.h"
#include "PortConfig.h"

#include <iostream>
#include <dbus/dbus.h>

using namespace std;

namespace Rgb {

class DbusClient {
protected:
	DBusError dbusError;
	DBusConnection* dbusConnection;
	list<std::shared_ptr<ControllerConfig>> controllerList;
public:
	DbusClient(string name);
	virtual ~DbusClient();

	list<std::shared_ptr<ControllerConfig>>* getControllerList();
	bool sendConfiguration(string controllerName);
	bool sendConfigurationPort(string controllerName, uint32_t portIndex, PortConfig portConfig);
	bool sendConfigurationEffect(string controllerName, uint32_t portIndex, uint32_t effectType, uint32_t effectDuration);
	bool sendConfigurationColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, PortConfigColor color);
	void updateControllers();
	void updateConfiguration(string controllerName, std::shared_ptr<ControllerConfig> controller);
	void setEffect(string controllerName, uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration);
	void setColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
};

} /* namespace Rgb */

#endif /* DBUSCLIENT_H_ */
