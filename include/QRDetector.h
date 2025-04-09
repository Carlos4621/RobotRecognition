#pragma once
#ifndef QRDETECTOR_H
#define QRDETECTOR_H

#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <QLabel>
#include <opencv4/opencv2/opencv.hpp>

/// @brief Clase capaz de detectar y decodificar códigos QR
class QRDetector : public QObject {

    Q_OBJECT

    using QRData = std::pair<QString, cv::Rect>;

public:

    /// @brief QRDetector Constructor base
    /// @param parent Padre del widget
    explicit QRDetector(QWidget* parent = nullptr);

    /// @brief detectQRsOnMat Envía una imágen para la detección de códigos QR, se emite una señal QRDecoded cuando se completa la tarea
    /// @param image Imagen a detectar
    void detectQRsOnMat(const cv::Mat& image);

signals:

    /// @brief QRDecoded Señal emitida cuando se termina de analizar la Mat enbusca de códigos QR
    /// @param QRDecoded Código QR decodificado
    /// @param QRArea Rectángulo que contiene el código QR
    void QRDecoded(QString QRDecoded, cv::Rect QRArea);

private slots:

    void emitIfQRFounded();

private:

    QFutureWatcher<QRData>* QRDetectorWorker_m;

    cv::QRCodeDetector QRDetector_m;

    void startQRWorker(const cv::Mat &image);
};

#endif // QRDETECTOR_H
