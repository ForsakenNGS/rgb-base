/*
 * ControllerConfig.cpp
 *
 *  Created on: Jan 3, 2019
 *      Author: forsaken
 */

#include "ControllerInfo.h"

namespace Rgb {

ControllerInfo::ControllerInfo(string portName): controllerPort(), controllerConfig() {
	// Initialize members
	this->portName = portName;
	this->version.major = 0;
	this->version.minor = 0;
	this->version.patch = 0;
}

ControllerInfo::~ControllerInfo() {

}

string ControllerInfo::getPortName() {
	return this->portName;
}

map<uint32_t,uint32_t>* ControllerInfo::getPortList() {
	return &this->controllerPort;
}

map<uint32_t,ControllerConfig>* ControllerInfo::getConfigList() {
	return &this->controllerConfig;
}

int ControllerInfo::getVerionMajor() {
	return this->version.major;
}

int ControllerInfo::getVerionMinor() {
	return this->version.minor;
}

int ControllerInfo::getVerionPatch() {
	return this->version.patch;
}

string ControllerInfo::getVersionString() {
	ostringstream versionString;
	versionString << this->version.major << "." << this->version.minor << "." << this->version.patch;
	return versionString.str();
}

void ControllerInfo::setConfig(uint32_t portIndex, uint32_t configIndex) {
	this->controllerPort[portIndex] = configIndex;
}

void ControllerInfo::setEffect(uint32_t configIndex, ControllerConfigType effectType, uint32_t effectDuration) {
	ControllerConfig config;
	if (this->controllerConfig.count(configIndex) > 0) {
		config = this->controllerConfig[configIndex];
	}
	config.setEffectType( effectType );
	config.setEffectDuration( effectDuration );
	this->controllerConfig[configIndex] = config;
}

void ControllerInfo::setColor(uint32_t configIndex, uint32_t colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
	ControllerConfig config;
	ControllerConfigColor color;
	if (this->controllerConfig.count(configIndex) > 0) {
		config = this->controllerConfig[configIndex];
	}
	color.red = red;
	color.green = green;
	color.blue = blue;
	config.setColor(colorIndex, color);
	this->controllerConfig[configIndex] = config;
}

void ControllerInfo::setVersion(uint32_t major, uint32_t minor, uint32_t patch) {
	this->version.major = major;
	this->version.minor = minor;
	this->version.patch = patch;
}

} /* namespace Rgb */
