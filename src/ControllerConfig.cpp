/*
 * ControllerConfig.cpp
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#include "ControllerConfig.h"

namespace Rgb {

ControllerConfig::ControllerConfig(string portName): controllerPorts() {
	// Initialize members
	this->portName = portName;
	this->version.major = 0;
	this->version.minor = 0;
	this->version.patch = 0;
}

ControllerConfig::~ControllerConfig() {

}

string ControllerConfig::getPortName() {
	return this->portName;
}

map<uint32_t,PortConfig>* ControllerConfig::getPortList() {
	return &this->controllerPorts;
}

int ControllerConfig::getVerionMajor() {
	return this->version.major;
}

int ControllerConfig::getVerionMinor() {
	return this->version.minor;
}

int ControllerConfig::getVerionPatch() {
	return this->version.patch;
}

string ControllerConfig::getVersionString() {
	ostringstream versionString;
	versionString << this->version.major << "." << this->version.minor << "." << this->version.patch;
	return versionString.str();
}

void ControllerConfig::setEffect(uint32_t portIndex, PortConfigType effectType, uint32_t effectDuration) {
	PortConfig config;
	if (this->controllerPorts.count(portIndex) > 0) {
		config = this->controllerPorts[portIndex];
	}
	config.setEffectType( effectType );
	config.setEffectDuration( effectDuration );
	this->controllerPorts[portIndex] = config;
}

void ControllerConfig::setColor(uint32_t portIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
	PortConfig config;
	PortConfigColor color;
	if (this->controllerPorts.count(portIndex) > 0) {
		config = this->controllerPorts[portIndex];
	}
	color.red = red;
	color.green = green;
	color.blue = blue;
	config.setColor(colorIndex, color);
	this->controllerPorts[portIndex] = config;
}

void ControllerConfig::setVersion(uint32_t major, uint32_t minor, uint32_t patch) {
	this->version.major = major;
	this->version.minor = minor;
	this->version.patch = patch;
}

} /* namespace Rgb */
