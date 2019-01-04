/*
 * PortConfig.h
 *
 *  Created on: Jan 2, 2019
 *      Author: forsaken
 */

#ifndef PORTCONFIG_H_
#define PORTCONFIG_H_

#include <cstdint>
#include <map>

using namespace std;

namespace Rgb {

enum PortConfigType : uint8_t {
	EFFECT_STATIC = 0,
	EFFECT_FADE = 1,
	EFFECT_BLINK = 2
};

struct PortConfigColor {
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
};

class PortConfig {
protected:
	PortConfigType				effectType;
	uint16_t					effectDuration;
	uint8_t						effectColor;
	map<int,PortConfigColor>	colorList;
public:
	PortConfig();
	virtual ~PortConfig();

	PortConfigType getEffectType();
	uint16_t getEffectDuration();
	uint8_t getEffectColorIndex();
	PortConfigColor getColor(int index);
	uint8_t getColorCount();

	void setEffectType(PortConfigType type);
	void setEffectDuration(uint16_t duration);
	void setEffectColorIndex(uint8_t colorIndex);
	void setColor(int index, PortConfigColor color);
};

} /* namespace Rgb */

#endif /* PORTCONFIG_H_ */
