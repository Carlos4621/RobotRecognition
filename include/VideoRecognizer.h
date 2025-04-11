#ifndef VIDEO_RECOGNIZER_HEADER
#define VIDEO_RECOGNIZER_HEADER

#include <QWidget>
#include <opencv4/opencv2/opencv.hpp>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "QRDetector.h"
#include "MovementDetector.h"
#include "YOLOv8Model.h"

namespace Ui {
class VideoRecognizer;
}

class VideoRecognizer : public QWidget {

    Q_OBJECT

    static const cv::Scalar QRRectangleColor;
    static const cv::Scalar MovementRectagleColor;
    static const cv::Scalar HazmatRectangleColor;

public:

    explicit VideoRecognizer(QWidget *parent = nullptr);
    ~VideoRecognizer();

private slots:

    void displayDetectedQRs(QString QRDecoded, cv::Rect boundingRectangle);
    void displayDetectedMovement(std::vector<cv::Rect> boundingRectangles);
    void displayDetectedHazmars(std::vector<PredictionsData> predictions);

    void onDetectMovementCheckboxChanged(const Qt::CheckState &newState);
    void onDetectQRCheckboxChanged(const Qt::CheckState& newState);
    void onDetectHazmatCheckboxChanged(const Qt::CheckState& newState);

private:

    Ui::VideoRecognizer* ui;

    QRDetector* QRDetector_m;
    MovementDetector* movementDetector_m;
    YOLOv8Model* hazmatDetector_m;

    void detectQRIfChecked(cv::Mat frame);
    void detectMovementIfChecked(cv::Mat frame);
    void detectHazmatIfChecked(cv::Mat frame);
};

#endif // VIDEO_RECOGNIZER_HEADER
