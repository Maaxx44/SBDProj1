#include "record.h"


// constructors
record::record(double angle = 0.0, double radius = 0.0) : angle(angle), radius(radius) {}

//public members
void record::setAngle(double newAngle) { this->angle = newAngle; }
void record::setRadius(double newRadius) { this->radius = newRadius; }

double record::getAngle() const { return this->angle; }
double record::getRadius() const { return this->radius; }

double record::calculateArea() const {
	if (this->angle == 0 || this->radius == 0) return 0.0;
	else return (M_PI * pow(this->radius, 2)) * (this->angle / 360.0);

}