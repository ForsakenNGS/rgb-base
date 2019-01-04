/*
 * DbusClient.cpp
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#include "DbusClient.h"

namespace Rgb {

DbusClient::DbusClient(string name): dbusError(), dbusConnection(NULL), controllerList() {
	dbus_error_init(&this->dbusError);
	this->dbusConnection = dbus_bus_get(DBUS_BUS_SESSION, &this->dbusError);
	int dbusNameResult = dbus_bus_request_name(this->dbusConnection, name.c_str(), DBUS_NAME_FLAG_ALLOW_REPLACEMENT | DBUS_NAME_FLAG_REPLACE_EXISTING, &this->dbusError);
	if (dbusNameResult != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		cout << "WARNING! Not primary owner of dbus name 'org.forsaken.RgbService'!\n";
	}
}

DbusClient::~DbusClient() {

}

list<std::shared_ptr<ControllerConfig>>* DbusClient::getControllerList() {
	return &this->controllerList;
}

bool DbusClient::sendConfiguration(string controllerName) {
	for (list<std::shared_ptr<ControllerConfig>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			map<uint32_t,PortConfig>* controllerPorts = i->get()->getPortList();
			for (map<uint32_t,PortConfig>::iterator itPort = controllerPorts->begin(),
					itPortEnd = controllerPorts->end(); itPort != itPortEnd; itPort++) {
				if (!this->sendConfigurationPort( controllerName, itPort->first, itPort->second )) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

bool DbusClient::sendConfigurationPort(string controllerName, uint32_t portIndex, PortConfig portConfig) {
	if (!this->sendConfigurationEffect(controllerName, portIndex, portConfig.getEffectType(), portConfig.getEffectDuration())) {
		return false;
	}
	uint32_t colorCount = portConfig.getColorCount();
	for (uint32_t colorIndex = 0; colorIndex < colorCount; colorIndex++) {
		if (!this->sendConfigurationColor(controllerName, portIndex, colorIndex, portConfig.getColor(colorIndex))) {
			return false;
		}
	}
	return true;
}

bool DbusClient::sendConfigurationEffect(string controllerName, uint32_t portIndex, uint32_t effectType, uint32_t effectDuration) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "setEffect");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_uint32_t controllerPortIndex = portIndex;
	dbus_uint32_t controllerEffectType = effectType;
	dbus_uint32_t controllerEffectDuration = effectDuration;
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerPortIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerEffectType);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerEffectDuration);
	// -> Send it
	dbus_connection_send_with_reply(this->dbusConnection, msg, &pending, -1);
	dbus_connection_flush(this->dbusConnection);
	dbus_message_unref(msg);
	dbus_pending_call_block(pending);
	// Obtain reply
	DBusMessage* reply = dbus_pending_call_steal_reply(pending);
	DBusMessageIter replyArgs;
	dbus_pending_call_unref(pending);
	dbus_message_iter_init(reply, &replyArgs);
	// Process reply
	dbus_bool_t result = false;
	dbus_message_iter_get_basic(&replyArgs, &result);
	dbus_message_unref(reply);
	return result;
}

bool DbusClient::sendConfigurationColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, PortConfigColor color) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "setColor");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_uint32_t controllerPortIndex = portIndex;
	dbus_uint32_t controllerColorIndex = colorIndex;
	dbus_uint32_t controllerColorRed = color.red;
	dbus_uint32_t controllerColorGreen = color.green;
	dbus_uint32_t controllerColorBlue = color.blue;
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerPortIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerColorIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerColorRed);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerColorGreen);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerColorBlue);
	// -> Send it
	dbus_connection_send_with_reply(this->dbusConnection, msg, &pending, -1);
	dbus_connection_flush(this->dbusConnection);
	dbus_message_unref(msg);
	dbus_pending_call_block(pending);
	// Obtain reply
	DBusMessage* reply = dbus_pending_call_steal_reply(pending);
	DBusMessageIter replyArgs;
	dbus_pending_call_unref(pending);
	dbus_message_iter_init(reply, &replyArgs);
	// Process reply
	dbus_bool_t result = false;
	dbus_message_iter_get_basic(&replyArgs, &result);
	dbus_message_unref(reply);
	return result;
}

void DbusClient::updateControllers() {
	this->controllerList.clear();
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "getControllers");
	DBusPendingCall* pending;
	dbus_connection_send_with_reply(this->dbusConnection, msg, &pending, -1);
	dbus_connection_flush(this->dbusConnection);
	dbus_message_unref(msg);
	dbus_pending_call_block(pending);
	// Obtain reply
	DBusMessage* reply = dbus_pending_call_steal_reply(pending);
	DBusMessageIter replyArgs;
	dbus_pending_call_unref(pending);
	dbus_message_iter_init(reply, &replyArgs);
	// Process reply
	dbus_uint32_t controllerCount = 0;
	char* controllerNameCstr = NULL;
	dbus_uint32_t controllerVersionMajor = 0;
	dbus_uint32_t controllerVersionMinor = 0;
	dbus_uint32_t controllerVersionPatch = 0;
	dbus_message_iter_get_basic(&replyArgs, &controllerCount);
	dbus_message_iter_next(&replyArgs);
	for (uint32_t i = 0; i < controllerCount; i++) {
		dbus_message_iter_get_basic(&replyArgs, &controllerNameCstr);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &controllerVersionMajor);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &controllerVersionMinor);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &controllerVersionPatch);
		dbus_message_iter_next(&replyArgs);
		string controllerName(controllerNameCstr);
		std::shared_ptr<ControllerConfig> controller = std::make_shared<ControllerConfig>(controllerName);
		controller->setVersion(controllerVersionMajor, controllerVersionMinor, controllerVersionPatch);
		this->controllerList.push_back(controller);
		updateConfiguration(controllerName, controller);
	}
	dbus_message_unref(reply);
}

void DbusClient::updateConfiguration(string controllerName, std::shared_ptr<ControllerConfig> controller) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "getConfig");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	// -> Send it
	dbus_connection_send_with_reply(this->dbusConnection, msg, &pending, -1);
	dbus_connection_flush(this->dbusConnection);
	dbus_message_unref(msg);
	dbus_pending_call_block(pending);
	// Obtain reply
	DBusMessage* reply = dbus_pending_call_steal_reply(pending);
	DBusMessageIter replyArgs;
	dbus_pending_call_unref(pending);
	dbus_message_iter_init(reply, &replyArgs);
	dbus_uint32_t portCount = 0;
	dbus_message_iter_get_basic(&replyArgs, &portCount);
	dbus_message_iter_next(&replyArgs);
	for (uint32_t portIndex = 0; portIndex < portCount; portIndex++) {
		dbus_uint32_t effectType = 0;
		dbus_uint32_t effectDuration = 0;
		dbus_uint32_t effectColorCount = 0;
		dbus_message_iter_get_basic(&replyArgs, &effectType);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &effectDuration);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &effectColorCount);
		dbus_message_iter_next(&replyArgs);
		controller->setEffect(portIndex, (PortConfigType)effectType, effectDuration);
		for (uint32_t colorIndex = 0; colorIndex < effectColorCount; colorIndex++) {
			dbus_uint32_t effectColorRed = 0;
			dbus_uint32_t effectColorGreen = 0;
			dbus_uint32_t effectColorBlue = 0;
			dbus_message_iter_get_basic(&replyArgs, &effectColorRed);
			dbus_message_iter_next(&replyArgs);
			dbus_message_iter_get_basic(&replyArgs, &effectColorGreen);
			dbus_message_iter_next(&replyArgs);
			dbus_message_iter_get_basic(&replyArgs, &effectColorBlue);
			dbus_message_iter_next(&replyArgs);
			controller->setColor(portIndex, colorIndex, effectColorRed, effectColorGreen, effectColorBlue);
		}
	}
	dbus_message_unref(reply);
}

void DbusClient::setEffect(string controllerName, uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration) {
	for (list<std::shared_ptr<ControllerConfig>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			i->get()->setEffect(portIndex, effectType, effectDuration);
			return;
		}
	}
}

void DbusClient::setColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
	for (list<std::shared_ptr<ControllerConfig>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			i->get()->setColor(portIndex, colorIndex, red, green, blue);
			return;
		}
	}
}

} /* namespace Rgb */
