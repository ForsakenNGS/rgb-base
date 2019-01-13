/*
 * PortConfig.h
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#ifndef CONTROLLERCONFIG_H_
#define CONTROLLERCONFIG_H_

#include <cstdint>
#include <map>

using namespace std;

namespace Rgb {

enum ControllerConfigType : uint8_t {
	EFFECT_STATIC = 0,
	EFFECT_FADE = 1,
	EFFECT_BLINK = 2
};

struct ControllerConfigColor {
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
};

class ControllerConfig {
protected:
	ControllerConfigType			effectType;
	uint16_t						effectDuration;
	uint8_t							effectColor;
	map<int,ControllerConfigColor>	colorList;
public:
	ControllerConfig();
	virtual ~ControllerConfig();

	ControllerConfigType getEffectType();
	uint16_t getEffectDuration();
	uint8_t getEffectColorIndex();
	ControllerConfigColor getColor(int index);
	uint8_t getColorCount();

	void setEffectType(ControllerConfigType type);
	void setEffectDuration(uint16_t duration);
	void setEffectColorIndex(uint8_t colorIndex);
	void setColor(int index, ControllerConfigColor color);
};

} /* namespace Rgb */

#endif /* CONTROLLERCONFIG_H_ */
