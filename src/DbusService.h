/*
 * DbusService.h
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#ifndef DBUSSERVICE_H_
#define DBUSSERVICE_H_

#include "Collection.h"

#include <dbus/dbus.h>
#include <boost/thread/thread.hpp>

namespace Rgb {

class DbusService {
protected:
	Rgb::Collection collection;
	DBusError dbusError;
	DBusConnection* dbusConnection;
	DBusHandleMessageFunction dbusFilter;

	void run();
public:
	DbusService();
	virtual ~DbusService();

	void handleMessage(DBusMessage *message);
	void operator()() {
		this->run();
	};
};

} /* namespace Rgb */

#endif /* DBUSSERVICE_H_ */
