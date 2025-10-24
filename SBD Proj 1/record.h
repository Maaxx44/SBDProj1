#pragma once
#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif
#include <random>
#include <cmath>

#include "globalData.cpp"

struct record {
private:
	double angle;
	double radius;
	double area; // precalculating area to optimize further calculations

	void recalculateArea();

public:
	record();
	record(double angle, double radius);

	void setAngle(double newAngle);
	void setRadius(double newRadius);

	double getAngle() const;
	double getRadius() const;
	double getArea() const;

	record static getRandomRecord();
};

