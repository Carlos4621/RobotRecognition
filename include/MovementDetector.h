#pragma once
#ifndef MOVEMENTDETECTOR_H
#define MOVEMENTDETECTOR_H

#include <QObject>
#include <opencv4/opencv2/opencv.hpp>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

/// @brief Clase capaz de detectar movimiento en una serie de Mat
class MovementDetector : public QObject {

    Q_OBJECT

    static constexpr size_t RectangleMinSize{ 5000 };

public:

    /// @brief MovementDetector Constructor base
    /// @param parent Padre del widget
    explicit MovementDetector(QObject *parent = nullptr);

    /// @brief addImage Añade una imagen para la detección de movimiento, en caso de detectarlo emite movementDetected
    /// @param image Imagen a añadir
    void addImage(const cv::Mat& image);

signals:

    /// @brief movementDetected Emitido cada vez que se detecta movimiento
    /// @param boundingRect Cuadros contenedores del movimiento
    void movementDetected(std::vector<cv::Rect> boundingRects);

private slots:

    void emitIfMovementDetected();

private:

    QFutureWatcher<std::vector<cv::Rect>>* movementDetectorWorker_m;

    cv::Ptr<cv::BackgroundSubtractor> backgroundSubstractor_m;

    void startWorker(cv::Mat image);

};

#endif // MOVEMENTDETECTOR_H
