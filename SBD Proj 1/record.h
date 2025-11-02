#pragma once
#include "common.h"

struct record {
private:
	double angle;
	double radius;

public:
	record(double angle = 0.0, double radius = 0.0);

	void setAngle(double newAngle);
	void setRadius(double newRadius);

	double getAngle() const;
	double getRadius() const;

	double calculateArea() const;

	record static getRandomRecord() {
		double randomAngle = randomAngleMin + ((rand() % 100) / (randomAngleMax - randomAngleMin));
		double randomRadius = randomRadiusMin + ((rand() % 100) / (randomRadiusMax - randomRadiusMin));

		return record(randomAngle, randomRadius);
	}
};

