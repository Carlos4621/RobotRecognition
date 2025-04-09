#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <opencv4/opencv2/opencv.hpp>

class Camera : public QObject {

    Q_OBJECT

public:

    explicit Camera(QObject *parent = nullptr);

signals:

    void newFrame();

private:


};

#endif // CAMERA_H
