/*
 * Collection.cpp
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#include "Collection.h"

namespace Rgb {

Collection::Collection() : controllerList() {
	this->portList = NULL;
}

Collection::~Collection() {
	this->close();
	this->freePortList();
}

void Collection::freePortList() {
	if (this->portList != NULL) {
		sp_free_port_list(this->portList);
		this->portList = NULL;
	}
}

void Collection::detectControllers() {
	this->freePortList();
	sp_return result = sp_list_ports(&this->portList);
	if (result != SP_OK) {
		throw new Exceptions::PortListFailed(result);
	}
	for (int i = 0; this->portList[i]; i++) {
		try {
			std::shared_ptr<Controller> controller = std::make_shared<Controller>(
				string(sp_get_port_name(this->portList[i]))
			);
			controller->open();
            this->controllerList.push_back( controller );
		} catch (Exceptions::PortException &exception) {
			// Failed to open/setup serial port
		} catch (Exceptions::ControllerNotFound &exception) {
			// No RGB controller found on the serial port
		}
	}
}

Controller* Collection::getController(uint32_t index) {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		if (index-- == 0) {
			return i->get();
		}
	}
	return NULL;
}

Controller* Collection::getController(string port) {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		return i->get();
	}
	return NULL;
}

int Collection::getControllerCount() {
	return (int)this->controllerList.size();
}

void Collection::open() {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		i->get()->open();
	}
}

void Collection::close() {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		i->get()->close();
	}
}

void Collection::update() {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		i->get()->update();
	}
}

void Collection::writeCommand(string command) {
	for (list<std::shared_ptr<Controller>>::iterator i = this->controllerList.begin(),
			end = this->controllerList.end(); i != end; i++) {
		i->get()->writeCommand(command);
	}
}

} /* namespace Rgb */
