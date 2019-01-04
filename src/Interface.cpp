/*
 * Interface.cpp
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#include "Interface.h"

namespace Rgb {

	Interface::~Interface() {

	}

	void Interface::getConfiguration() {
		this->writeCommand("?");
	}

	void Interface::setEffect(uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration) {
		stringstream command;
		command << hex << setfill('0')
				<< "P" << setw(1) << portIndex
				<< "ET" << setw(1) << effectType
				<< "ED" << setw(4) << effectDuration;
		this->writeCommand(command.str());
	}

	void Interface::setColor(uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
		stringstream command;
		command << hex << setfill('0')
				<< "P" << setw(1) << (uint32_t)portIndex
				<< "C" << setw(1) << (uint32_t)colorIndex
				<< "#" << setw(2) << (uint32_t)red << (uint32_t)green << (uint32_t)blue;
		this->writeCommand(command.str());
	}

} /* namespace Rgb */
