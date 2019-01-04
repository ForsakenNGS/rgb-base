/*
 * PortConfig.cpp
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#include "PortConfig.h"

namespace Rgb {

PortConfig::PortConfig(): colorList() {
	this->effectType = EFFECT_STATIC;
	this->effectDuration = 0;
	this->effectColor = 0;
}

PortConfig::~PortConfig() {

}

PortConfigType PortConfig::getEffectType() {
	return this->effectType;
}

uint16_t PortConfig::getEffectDuration() {
	return this->effectDuration;
}

uint8_t PortConfig::getEffectColorIndex() {
	return this->effectColor;
}

PortConfigColor PortConfig::getColor(int index) {
	PortConfigColor color;
	if (this->colorList.count(index) > 0) {
		color = this->colorList[index];
	}
	return color;
}

uint8_t PortConfig::getColorCount() {
	return this->colorList.size();
}

void PortConfig::setEffectType(PortConfigType type) {
	this->effectType = type;
}

void PortConfig::setEffectDuration(uint16_t duration) {
	this->effectDuration = duration;
}

void PortConfig::setEffectColorIndex(uint8_t colorIndex) {
	this->effectColor = colorIndex;
}

void PortConfig::setColor(int index, PortConfigColor color) {
	this->colorList[index] = color;
}

} /* namespace Rgb */
