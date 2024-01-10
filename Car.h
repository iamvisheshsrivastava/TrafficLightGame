// Car.h
#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Car : public QObject
{
    Q_OBJECT
public:
    enum DriverType { A, B, C };

    explicit Car(DriverType type, double maxAcceleration, QObject *parent = nullptr);

    void accelerate(double time);
    void brake(double time);
    void updatePosition(double time);
    void updateGraphics();
    void setPosition(double position);
    void setVelocity(double velocity);
    double position() const;
    double speed() const;
    DriverType driverType() const;

    void setGraphicsItem(QGraphicsPixmapItem* item) { graphicsItem_ = item; }
    QGraphicsPixmapItem* graphicsItem() const { return graphicsItem_; }

private:
    DriverType driverType_;
    double maxAcceleration_;
    double currentSpeed_;
    double currentPosition_;
    QGraphicsPixmapItem* graphicsItem_;
};

#endif // CAR_H
