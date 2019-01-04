/*
 * Interface.h
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <ios>
#include <iomanip>
#include <string>
#include <cstdint>
#include <libserialport.h>

#include "PortConfig.h"

using namespace std;

namespace Rgb {

class Interface {
protected:
	uint32_t portIndex = 0;
	uint32_t colorIndex = 0;
public:
	virtual ~Interface();

	virtual void close() = 0;
	virtual void open() = 0;
	virtual void update() = 0;
	virtual void writeCommand(std::string command) = 0;

	void getConfiguration();
	void setEffect(uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration);
	void setColor(uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
};

} /* namespace Rgb */

#endif /* INTERFACE_H_ */
