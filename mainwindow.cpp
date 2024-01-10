// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QRandomGenerator>
#include <QGridLayout>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , simulationRunning_(false)
    , simulationTimer_(new QTimer(this))
    , scene_(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new QGridLayout);
    ui->centralwidget->layout()->addWidget(ui->simulationView);
    ui->simulationView->setScene(scene_);
    connect(ui->startSimulationButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::updateSimulation);
    ui->simulationView->setFixedSize(800, 500);
    scene_->setSceneRect(0, 0, 800, 500);
}

MainWindow::~MainWindow()
{
    cleanup();
    delete ui;
}

void MainWindow::startSimulation()
{
    cleanup();

    if(simulationRunning_) {
        QMessageBox::warning(this, "Simulation Running", "The simulation is already running.");
        return;
    }

    bool ok1, ok2;
    routeLength_ = ui->routeLengthInput->text().toDouble(&ok1);
    int numberOfCars = ui->numberOfCarsInput->text().toInt(&ok2);  // Assuming you have a QLineEdit named numberOfCarsInput
    if(!ok1 || routeLength_ <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid route length.");
        return;
    }
    if(!ok2 || numberOfCars <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid number of cars.");
        return;
    }

    scene_->setSceneRect(0, 0, routeLength_, 600);
    ui->simulationView->setSceneRect(0, 0, routeLength_, 600);

    double initialCarPosition = 0.0;
    double carSeparation = 50.0;
    double maxAcceleration = 3.0;  // Assuming all cars have the same maximum acceleration

    for (int i = 0; i < numberOfCars; ++i) {
        Car::DriverType driverType = static_cast<Car::DriverType>(i % 3);  // Cycle through the 3 driver types
        Car* car = new Car(driverType, maxAcceleration, this);
        car->setPosition(initialCarPosition);
        cars_.append(car);

        QPixmap carPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/car.jpg");
        carPixmap = carPixmap.scaled(37, 37, Qt::KeepAspectRatio);
        QGraphicsPixmapItem *carItem = new QGraphicsPixmapItem();
        carItem->setPixmap(carPixmap);
        car->setGraphicsItem(carItem);
        carItem->setPos(car->position(), i * carSeparation);  // Adjust vertical position
        scene_->addItem(carItem);
    }

    // Create a single traffic light at the bottom
    TrafficLight* light = new TrafficLight(200, this);  // Assuming the light is at position 300
    QPixmap lightPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/green.jpg");
    lightPixmap = lightPixmap.scaled(25, 25, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QGraphicsPixmapItem *lightItem = new QGraphicsPixmapItem();
    lightItem->setPixmap(lightPixmap);
    light->setGraphicsItem(lightItem);
    lightItem->setPos(light->position(), 250);  // Adjust vertical position to be at the bottom
    scene_->addItem(lightItem);
    trafficLights_.append(light);

    simulationRunning_ = true;
    simulationTimer_->start(100);
}

void MainWindow::updateSimulation()
{
    double maxAcceleration = 3.0;  // m/s^2, assuming all cars have the same maximum acceleration

    TrafficLight* light = trafficLights_.first();  // Assuming there is only one traffic light

    static QTime time = QTime::currentTime().addSecs(0);  // Initialize with a time two seconds in the past

    if (QTime::currentTime() > time.addSecs(4)) {  // Check if 2 seconds have passed
        time = QTime::currentTime();  // Update the time
        light->setGreen(false);  // Set the light to red
        QPixmap redPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/red.jpg");
        redPixmap = redPixmap.scaled(25, 25, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QGraphicsPixmapItem* lightItem = static_cast<QGraphicsPixmapItem*>(light->graphicsItem());
        lightItem->setPixmap(redPixmap);

        QTimer::singleShot(3000, this, [this, light]() {
            light->setGreen(true);  // Set the light back to green after 1 more second
            QPixmap greenPixmap("C:/Users/Vishesh Srivastava/Desktop/Delete/Traffic/green.jpg");
            greenPixmap = greenPixmap.scaled(25, 25, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QGraphicsPixmapItem* lightItem = static_cast<QGraphicsPixmapItem*>(light->graphicsItem());
            lightItem->setPixmap(greenPixmap);
        });
    }

    for (int i = 0; i < cars_.size(); ++i) {
        Car* car = cars_[i];

        if (!light->isGreen()) {
            // If the light is red, stop the car
            car->setVelocity(0.0);
        } else {
            // If the light is green, the car can accelerate normally
            car->accelerate(0.6);  // Assuming a time step of 0.6 seconds
        }

        car->updatePosition(0.3);  // Assuming a time step of 0.1 seconds
        if (car->graphicsItem()) {
            QGraphicsPixmapItem* carItem = static_cast<QGraphicsPixmapItem*>(car->graphicsItem());
            carItem->setPos(car->position(), carItem->y());
        }
    }

    update();  // Update the MainWindow, triggering a repaint

    bool allCarsFinished = true;
    for (Car* car : cars_) {
        if (car->position() < routeLength_) {
            allCarsFinished = false;
            break;
        }
    }
    if (allCarsFinished) {
        simulationTimer_->stop();
        simulationRunning_ = false;
        QMessageBox::information(this, "Simulation Complete", "All cars have reached the end of the route.");
    }
}

void MainWindow::cleanup()
{
    for (Car* car : cars_) {
        scene_->removeItem(car->graphicsItem());
        delete car->graphicsItem();
    }
    for (TrafficLight* light : trafficLights_) {
        scene_->removeItem(light->graphicsItem());
        delete light->graphicsItem();
    }
    qDeleteAll(cars_);
    cars_.clear();
    qDeleteAll(trafficLights_);
    trafficLights_.clear();
}
