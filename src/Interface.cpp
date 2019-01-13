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

	void Interface::setConfig(uint32_t portIndex, uint32_t configIndex) {
		stringstream command;
		command << hex << setfill('0')
				<< "P" << setw(1) << portIndex << setw(1) << configIndex
				<< "\n";
		this->writeCommand(command.str());
	}

	void Interface::setEffect(uint32_t configIndex, ControllerConfigType effectType, uint32_t effectDuration) {
		stringstream command;
		command << hex << setfill('0')
				<< "S" << setw(1) << configIndex
				<< "ET" << setw(1) << effectType
				<< "ED" << setw(4) << effectDuration
				<< "\n";
		this->writeCommand(command.str());
	}

	void Interface::setColor(uint32_t configIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
		stringstream command;
		command << hex << setfill('0')
				<< "S" << setw(1) << (uint32_t)configIndex
				<< "C" << setw(1) << (uint32_t)colorIndex
				<< "#"
					<< setw(2) << (uint32_t)red
					<< setw(2) << (uint32_t)green
					<< setw(2) << (uint32_t)blue
				<< "\n";
		this->writeCommand(command.str());
	}

	void Interface::writeToEeprom() {
		stringstream command;
		command
				<< "W"
				<< "\n";
		this->writeCommand(command.str());
	}

} /* namespace Rgb */
