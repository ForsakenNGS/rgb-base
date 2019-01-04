/*
 * DbusService.cpp
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#include "DbusService.h"

namespace Rgb {

DbusService::DbusService(): collection(), dbusError(), dbusConnection(NULL), dbusFilter() {
	dbus_error_init(&this->dbusError);
	this->dbusConnection = dbus_bus_get(DBUS_BUS_SYSTEM, &this->dbusError);
}

DbusService::~DbusService() {
	dbus_error_free(&this->dbusError);
}

void DbusService::run() {
	try {
		DBusMessage* dbusMessage = NULL;
		// TODO: Remove DBUS_NAME_FLAG_ALLOW_REPLACEMENT flag when done testing!
		int dbusNameResult = dbus_bus_request_name(this->dbusConnection, "org.forsaken.RgbService", DBUS_NAME_FLAG_ALLOW_REPLACEMENT | DBUS_NAME_FLAG_REPLACE_EXISTING, &this->dbusError);
		if (dbusNameResult != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
			cout << "WARNING! Not primary owner of dbus name 'org.forsaken.RgbService'!\n";
		}
		//dbus_bus_add_match(this->dbusConnection, "type='method_call',interface='org.forsaken.RgbService',member='setColor'", &this->dbusError);
		//dbus_connection_add_filter(this->dbusConnection, DbusService::monitor_filter_func, NULL, NULL);
		//dbus_bus_start_service_by_name(this->dbusConnection, "org.forsaken.RgbService", 0, 0, &this->dbusError);
		this->collection.detectControllers();
		this->collection.getConfiguration();
		while (!boost::this_thread::interruption_requested()) {
			this->collection.update();
			dbus_connection_read_write(this->dbusConnection, 0);
			dbusMessage = dbus_connection_pop_message(this->dbusConnection);
			if (dbusMessage != NULL) {
				this->handleMessage(dbusMessage);
				dbus_message_unref(dbusMessage);
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	} catch (boost::thread_interrupted&) {
		DBusError dbusErrorRelease;
		this->collection.close();
		dbus_error_init(&dbusErrorRelease);
		dbus_bus_release_name(this->dbusConnection, "org.forsaken.RgbService", &dbusErrorRelease);
		dbus_error_free(&dbusErrorRelease);
	}
}

void DbusService::handleMessage(DBusMessage *message) {
	DBusMessageIter messageArgs;
	if (dbus_message_is_method_call(message, "org.forsaken.RgbController", "getControllers")) {
		// Process message
		dbus_message_iter_init(message, &messageArgs);
		dbus_uint32_t controllerCount = this->collection.getControllerCount();
		// Send reply
		DBusMessage* reply = dbus_message_new_method_return(message);
		DBusMessageIter replyArgs;
		dbus_uint32_t replySerial = 0;
		dbus_message_iter_init_append(reply, &replyArgs);
		dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &controllerCount);
		for (uint32_t i = 0; i < controllerCount; i++) {
			Controller* controller = this->collection.getController(i);
			const char* controllerName = controller->getPortName().c_str();
			dbus_uint32_t controllerVersionMajor = controller->getVerionMajor();
			dbus_uint32_t controllerVersionMinor = controller->getVerionMinor();
			dbus_uint32_t controllerVersionPatch = controller->getVerionPatch();
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_STRING, &controllerName);
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &controllerVersionMajor);
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &controllerVersionMinor);
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &controllerVersionPatch);
		}
		dbus_connection_send(this->dbusConnection, reply, &replySerial);
		dbus_connection_flush(this->dbusConnection);
		dbus_message_unref(reply);
	} else if (dbus_message_is_method_call(message, "org.forsaken.RgbController", "getConfig")) {
		/*
		 * Get a controllers current configuration
		 */
		// Process message
		dbus_message_iter_init(message, &messageArgs);
		char* controllerNameCstr;
		dbus_message_iter_get_basic(&messageArgs, &controllerNameCstr);
		string controllerName(controllerNameCstr);
		Controller* controller = this->collection.getController(controllerName);
		map<uint32_t,PortConfig>* controllerPorts = controller->getPortList();
		// Send reply
		DBusMessage* reply = dbus_message_new_method_return(message);
		DBusMessageIter replyArgs;
		dbus_uint32_t portCount = controllerPorts->size();
		dbus_uint32_t replySerial = 0;
		dbus_message_iter_init_append(reply, &replyArgs);
		dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &portCount);
		for (map<uint32_t,PortConfig>::iterator itPort = controllerPorts->begin(),
				itPortEnd = controllerPorts->end(); itPort != itPortEnd; itPort++) {
			dbus_uint32_t effectType = itPort->second.getEffectType();
			dbus_uint32_t effectDuration = itPort->second.getEffectDuration();
			dbus_uint32_t effectColorCount = itPort->second.getColorCount();
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectType);
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectDuration);
			dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectColorCount);
			for (uint32_t colorIndex = 0; colorIndex < effectColorCount; colorIndex++) {
				PortConfigColor color = itPort->second.getColor(colorIndex);
				dbus_uint32_t effectColorRed = color.red;
				dbus_uint32_t effectColorGreen = color.green;
				dbus_uint32_t effectColorBlue = color.blue;
				dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectColorRed);
				dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectColorGreen);
				dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_UINT32, &effectColorBlue);
			}
		}
		dbus_connection_send(this->dbusConnection, reply, &replySerial);
		dbus_connection_flush(this->dbusConnection);
		dbus_message_unref(reply);
	} else if (dbus_message_is_method_call(message, "org.forsaken.RgbController", "setColor")) {
		/*
		 * Set a color
		 */
		// Process message
		dbus_message_iter_init(message, &messageArgs);
		char* controllerNameCstr = NULL;
		dbus_uint32_t portIndex = 0;
		dbus_uint32_t colorIndex = 0;
		dbus_uint16_t red = 0;
		dbus_uint16_t green = 0;
		dbus_uint16_t blue = 0;
		dbus_message_iter_get_basic(&messageArgs, &controllerNameCstr);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &portIndex);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &colorIndex);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &red);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &green);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &blue);
		string controllerName(controllerNameCstr);
		if (controllerName.compare("*") == 0) {
			this->collection.setColor(portIndex, colorIndex, red, green, blue);
		} else {
			this->collection.getController(controllerName)->setColor(portIndex, colorIndex, red, green, blue);
		}
		// Send reply
		DBusMessage* reply = dbus_message_new_method_return(message);
		DBusMessageIter replyArgs;
		dbus_bool_t replySuccess = true;
		dbus_uint32_t replySerial = 0;
		dbus_message_iter_init_append(reply, &replyArgs);
		dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_BOOLEAN, &replySuccess);
		dbus_connection_send(this->dbusConnection, reply, &replySerial);
		dbus_connection_flush(this->dbusConnection);
		dbus_message_unref(reply);
	} else if (dbus_message_is_method_call(message, "org.forsaken.RgbController", "setEffect")) {
		/*
		 * Set an effect
		 */
		// Process message
		dbus_message_iter_init(message, &messageArgs);
		char* controllerNameCstr = NULL;
		dbus_uint32_t portIndex = 0;
		dbus_uint16_t effectType = 0;
		dbus_uint32_t effectDuration = 0;
		dbus_message_iter_get_basic(&messageArgs, &controllerNameCstr);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &portIndex);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &effectType);
		dbus_message_iter_next(&messageArgs);
		dbus_message_iter_get_basic(&messageArgs, &effectDuration);
		string controllerName(controllerNameCstr);
		if (controllerName.compare("*") == 0) {
			this->collection.setEffect(portIndex, (PortConfigType)effectType, effectDuration);
		} else {
			this->collection.getController(controllerName)->setEffect(portIndex, (PortConfigType)effectType, effectDuration);
		}
		// Send reply
		DBusMessage* reply = dbus_message_new_method_return(message);
		DBusMessageIter replyArgs;
		dbus_bool_t replySuccess = true;
		dbus_uint32_t replySerial = 0;
		dbus_message_iter_init_append(reply, &replyArgs);
		dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_BOOLEAN, &replySuccess);
		dbus_connection_send(this->dbusConnection, reply, &replySerial);
		dbus_connection_flush(this->dbusConnection);
		dbus_message_unref(reply);
	}
}

} /* namespace Rgb */
