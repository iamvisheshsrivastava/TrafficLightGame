#include "TrafficLight.h"

TrafficLight::TrafficLight(double position, QObject *parent)
    : QObject(parent), position_(position), green_(true), greenTimerActive_(false)
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer_->start(10000);  // Example: toggle state every 10 seconds
}

void TrafficLight::toggle()
{
    green_ = !green_;
}

bool TrafficLight::isGreen() const
{
    return green_;
}

double TrafficLight::position() const
{
    return position_;
}

void TrafficLight::onTimeout()
{
    toggle();
}

void TrafficLight::setGreen(bool green)  // <-- setGreen method
{
    green_ = green;
    updateGraphics();  // Update graphics when the light state changes
}

void TrafficLight::updateGraphics() {
    if (graphicsItem_) {
        QPixmap pixmap;
        if (isGreen()) {
            pixmap = QPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/green.jpg");
        } else {
            pixmap = QPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/red.jpg");
        }
        pixmap = pixmap.scaled(25, 25, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        graphicsItem_->setPixmap(pixmap);
    }
}

bool TrafficLight::isGreenTimerActive() const  // <-- isGreenTimerActive method
{
    return greenTimerActive_;
}

void TrafficLight::setGreenTimerActive(bool active)  // <-- setGreenTimerActive method
{
    greenTimerActive_ = active;
}
