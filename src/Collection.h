/*
 * Collection.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef COLLECTION_H_
#define COLLECTION_H_

#include "Interface.h"
#include "Controller.h"
#include "exceptions/PortListFailed.h"

#include <memory>

using namespace std;

namespace Rgb {

class Collection: public Interface {
protected:
	sp_port **portList;
	list<std::shared_ptr<Controller>> controllerList;

	void freePortList();
public:
	Collection();
	virtual ~Collection();

	void detectControllers();
	Controller* getController(uint32_t index);
	Controller* getController(string port);
	int getControllerCount();

	void close();
	void open();
	void update();
	void writeCommand(string command);
};

} /* namespace Rgb */

#endif /* COLLECTION_H_ */
