/*
 * PortConfig.cpp
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#include "ControllerConfig.h"

namespace Rgb {

ControllerConfig::ControllerConfig(): colorList() {
	this->effectType = EFFECT_STATIC;
	this->effectDuration = 0;
	this->effectColor = 0;
}

ControllerConfig::~ControllerConfig() {

}

ControllerConfigType ControllerConfig::getEffectType() {
	return this->effectType;
}

uint16_t ControllerConfig::getEffectDuration() {
	return this->effectDuration;
}

uint8_t ControllerConfig::getEffectColorIndex() {
	return this->effectColor;
}

ControllerConfigColor ControllerConfig::getColor(int index) {
	ControllerConfigColor color;
	if (this->colorList.count(index) > 0) {
		color = this->colorList[index];
	}
	return color;
}

uint8_t ControllerConfig::getColorCount() {
	return this->colorList.size();
}

void ControllerConfig::setEffectType(ControllerConfigType type) {
	this->effectType = type;
}

void ControllerConfig::setEffectDuration(uint16_t duration) {
	this->effectDuration = duration;
}

void ControllerConfig::setEffectColorIndex(uint8_t colorIndex) {
	this->effectColor = colorIndex;
}

void ControllerConfig::setColor(int index, ControllerConfigColor color) {
	this->colorList[index] = color;
}

} /* namespace Rgb */
