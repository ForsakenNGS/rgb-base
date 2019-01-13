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

list<std::shared_ptr<ControllerInfo>>* DbusClient::getControllerList() {
	return &this->controllerList;
}

bool DbusClient::sendConfiguration(string controllerName) {
	for (list<std::shared_ptr<ControllerInfo>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			map<uint32_t,uint32_t>* controllerPorts = i->get()->getPortList();
			for (map<uint32_t,uint32_t>::iterator itPort = controllerPorts->begin(),
					itPortEnd = controllerPorts->end(); itPort != itPortEnd; itPort++) {
				if (!this->sendConfigurationActive( controllerName, itPort->first, itPort->second )) {
					return false;
				}
			}
			map<uint32_t,ControllerConfig>* controllerConfigs = i->get()->getConfigList();
			for (map<uint32_t,ControllerConfig>::iterator itConfig = controllerConfigs->begin(),
					itConfigEnd = controllerConfigs->end(); itConfig != itConfigEnd; itConfig++) {
				if (!this->sendConfigurationSingle( controllerName, itConfig->first, itConfig->second )) {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

bool DbusClient::sendConfigurationSingle(string controllerName, uint32_t portIndex, ControllerConfig portConfig) {
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

bool DbusClient::sendConfigurationActive(string controllerName, uint32_t portIndex, uint32_t configIndex) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "setConfig");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_uint32_t controllerPortIndex = portIndex;
	dbus_uint32_t controllerConfigIndex = configIndex;
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerPortIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerConfigIndex);
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

bool DbusClient::sendConfigurationEffect(string controllerName, uint32_t configIndex, uint32_t effectType, uint32_t effectDuration) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "setEffect");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_uint32_t controllerConfigIndex = configIndex;
	dbus_uint16_t controllerEffectType = effectType;
	dbus_uint32_t controllerEffectDuration = effectDuration;
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerConfigIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT16, &controllerEffectType);
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

bool DbusClient::sendConfigurationColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, ControllerConfigColor color) {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "setColor");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = controllerName.c_str();
	dbus_uint32_t controllerPortIndex = portIndex;
	dbus_uint32_t controllerColorIndex = colorIndex;
	dbus_uint16_t controllerColorRed = color.red;
	dbus_uint16_t controllerColorGreen = color.green;
	dbus_uint16_t controllerColorBlue = color.blue;
	dbus_message_iter_init_append(msg, &msgArgs);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_STRING, &controllerNameCstr);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerPortIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT32, &controllerColorIndex);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT16, &controllerColorRed);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT16, &controllerColorGreen);
	dbus_message_iter_append_basic(&msgArgs, DBUS_TYPE_UINT16, &controllerColorBlue);
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

bool DbusClient::writeToEeprom() {
	// Send message
	DBusMessage* msg = dbus_message_new_method_call("org.forsaken.RgbService", "/org/forsaken/RgbController", "org.forsaken.RgbController", "writeToEeprom");
	DBusMessageIter msgArgs;
	DBusPendingCall* pending;
	// -> Add arguments
	const char* controllerNameCstr = "*";
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
		std::shared_ptr<ControllerInfo> controller = std::make_shared<ControllerInfo>(controllerName);
		controller->setVersion(controllerVersionMajor, controllerVersionMinor, controllerVersionPatch);
		this->controllerList.push_back(controller);
		updateConfiguration(controllerName, controller);
	}
	dbus_message_unref(reply);
}

void DbusClient::updateConfiguration(string controllerName, std::shared_ptr<ControllerInfo> controller) {
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
	dbus_uint32_t configCount = 0;
	// Read ports
	dbus_message_iter_get_basic(&replyArgs, &portCount);
	dbus_message_iter_next(&replyArgs);
	for (uint32_t portIndex = 0; portIndex < portCount; portIndex++) {
		dbus_uint32_t portId = 0;
		dbus_uint32_t configId = 0;
		dbus_message_iter_get_basic(&replyArgs, &portId);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &configId);
		dbus_message_iter_next(&replyArgs);
		controller->setConfig(portId, configId);
	}
	// Read configurations
	dbus_message_iter_get_basic(&replyArgs, &configCount);
	dbus_message_iter_next(&replyArgs);
	for (uint32_t configIndex = 0; configIndex < configCount; configIndex++) {
		dbus_uint16_t effectType = 0;
		dbus_uint32_t effectDuration = 0;
		dbus_uint32_t effectColorCount = 0;
		dbus_message_iter_get_basic(&replyArgs, &effectType);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &effectDuration);
		dbus_message_iter_next(&replyArgs);
		dbus_message_iter_get_basic(&replyArgs, &effectColorCount);
		dbus_message_iter_next(&replyArgs);
		controller->setEffect(configIndex, (ControllerConfigType)effectType, effectDuration);
		for (uint32_t colorIndex = 0; colorIndex < effectColorCount; colorIndex++) {
			dbus_uint16_t effectColorRed = 0;
			dbus_uint16_t effectColorGreen = 0;
			dbus_uint16_t effectColorBlue = 0;
			dbus_message_iter_get_basic(&replyArgs, &effectColorRed);
			dbus_message_iter_next(&replyArgs);
			dbus_message_iter_get_basic(&replyArgs, &effectColorGreen);
			dbus_message_iter_next(&replyArgs);
			dbus_message_iter_get_basic(&replyArgs, &effectColorBlue);
			dbus_message_iter_next(&replyArgs);
			controller->setColor(configIndex, colorIndex, effectColorRed, effectColorGreen, effectColorBlue);
		}
	}
	dbus_message_unref(reply);
}

void DbusClient::setConfig(string controllerName, uint32_t portIndex, uint32_t configIndex) {
	for (list<std::shared_ptr<ControllerInfo>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			i->get()->setConfig(portIndex, configIndex);
			return;
		}
	}
}

void DbusClient::setEffect(string controllerName, uint32_t portIndex, ControllerConfigType effectType, uint32_t effectDuration) {
	for (list<std::shared_ptr<ControllerInfo>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			i->get()->setEffect(portIndex, effectType, effectDuration);
			return;
		}
	}
}

void DbusClient::setColor(string controllerName, uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
	for (list<std::shared_ptr<ControllerInfo>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (i->get()->getPortName().compare(controllerName) == 0) {
			i->get()->setColor(portIndex, colorIndex, red, green, blue);
			return;
		}
	}
}

} /* namespace Rgb */
