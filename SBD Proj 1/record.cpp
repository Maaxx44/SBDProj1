#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include "record.h"
#include "globalData.hpp"
#include <random>
#include <cmath>

// private members
void record::recalculateArea() {
	if (this->angle == 0 || this->radius == 0) this->area = 0;
	else this->area = (M_PI * pow(this->radius, 2)) * (this->angle / 360.0);
}

// constructors
record::record() : angle(0), radius(0), area(0) {}
record::record(double angle, double radius) : angle(angle), radius(radius) {
	this->recalculateArea();
}

//public members
void record::setAngle(double newAngle) {
	this->angle = newAngle;
	this->recalculateArea();
}
void record::setRadius(double newRadius) {
	this->radius = newRadius;
	this->recalculateArea();
}

double record::getAngle() const { return this->angle; }
double record::getRadius() const { return this->radius; }
double record::getArea() const { return this->area; }

record static getRandomRecord() {
	return record(angleRandDistr(randomEngine), radiusRandDistr(randomEngine)); // i hope this works
}