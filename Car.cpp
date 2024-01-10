#include "Car.h"
#include <cmath>

Car::Car(DriverType type, double maxAcceleration, QObject *parent)
    : QObject(parent), driverType_(type), maxAcceleration_(maxAcceleration),
    currentSpeed_(0), currentPosition_(0)
{
}

void Car::accelerate(double time) {
    double acceleration;
    switch (driverType_) {
    case A:
        acceleration = (2.0 / 3) * maxAcceleration_;
        break;
    case B:
        acceleration = maxAcceleration_;
        break;
    case C:
        acceleration = 0.5 * maxAcceleration_;
        break;
    }
    currentSpeed_ += acceleration * time;
    double speedLimit = 50 * 1000 / 3600.0;  // Convert km/h to m/s
    if (driverType_ != C && currentSpeed_ > speedLimit) {
        currentSpeed_ = speedLimit;
    }
}

void Car::brake(double time)
{
    currentSpeed_ -= maxAcceleration_ * time;
    if (currentSpeed_ < 0) {
        currentSpeed_ = 0;
    }
}

void Car::updatePosition(double time)
{
    currentPosition_ += currentSpeed_ * time;
}

void Car::updateGraphics() {
    if (graphicsItem_) {
        graphicsItem_->setPos(position(), 0);
    }
}

void Car::setVelocity(double velocity) {
    currentSpeed_ = velocity;
}

double Car::position() const
{
    return currentPosition_;
}

double Car::speed() const
{
    return currentSpeed_;
}

Car::DriverType Car::driverType() const
{
    return driverType_;
}

void Car::setPosition(double position) {
    currentPosition_ = position;
}
