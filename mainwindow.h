#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include "Car.h"
#include "TrafficLight.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSimulation();
    void updateSimulation();

private:
    Ui::MainWindow *ui;
    QVector<Car*> cars_;
    QVector<TrafficLight*> trafficLights_;
    double routeLength_;
    bool simulationRunning_;
    QTimer* simulationTimer_;
    QGraphicsScene* scene_;  // Declare scene_ here

    void cleanup();
};

#endif // MAINWINDOW_H
