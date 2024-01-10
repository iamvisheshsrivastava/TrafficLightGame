// TrafficLight.h
#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QObject>
#include <QTimer>
#include <QGraphicsPixmapItem>  // Include this header

class TrafficLight : public QObject
{
    Q_OBJECT
public:
    explicit TrafficLight(double position, QObject *parent = nullptr);

    void toggle();
    bool isGreen() const;
    double position() const;
    void updateGraphics();
    void setGreen(bool green);
    void setGraphicsItem(QGraphicsPixmapItem* item) { graphicsItem_ = item; }  // Add this method
    QGraphicsPixmapItem* graphicsItem() const { return graphicsItem_; }  // Add this method

    bool isGreenTimerActive() const;  // <-- Add this method
    void setGreenTimerActive(bool active);  // <-- Add this method

private slots:
    void onTimeout();

private:
    double position_;
    bool green_;
    bool greenTimerActive_;  // <-- Add this member variable
    QTimer* timer_;
    QGraphicsPixmapItem* graphicsItem_;
};

#endif // TRAFFICLIGHT_H
