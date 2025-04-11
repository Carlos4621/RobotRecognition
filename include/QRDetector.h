#ifndef QR_DETECTOR_HEADER
#define QR_DETECTOR_HEADER

#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <QLabel>
#include <opencv4/opencv2/opencv.hpp>

/// @brief Clase capaz de detectar y decodificar códigos QR
class QRDetector : public QObject {

    Q_OBJECT

public:

    /// @brief QRDetector Constructor base
    /// @param parent Padre del widget
    explicit QRDetector(QWidget* parent = nullptr);

    /// @brief detectQRsOnMat Envía una imágen para la detección de códigos QR, se emite una señal QRDecoded cuando se completa la tarea
    /// @param image Imagen a detectar
    void detectQRsOnMat(const cv::Mat& image);

signals:

    /// @brief QRDecoded Señal emitida cuando se termina de analizar la Mat en busca de códigos QR
    /// @param QRDecoded Código QR decodificado
    /// @param QRArea Rectángulo que contiene el código QR
    void QRDecoded(QString QRDecoded, cv::Rect QRArea);

private slots:

    void emitIfQRFound();

private:

    using QRData = std::pair<QString, cv::Rect>;

    QFutureWatcher<QRData>* QRDetectorWorker_m;

    cv::QRCodeDetector QRDetector_m;

    void startQRWorker(const cv::Mat &image);
};

#endif // QR_DETECTOR_HEADER
