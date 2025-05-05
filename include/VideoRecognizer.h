#ifndef VIDEO_RECOGNIZER_HEADER
#define VIDEO_RECOGNIZER_HEADER

#include <QWidget>
#include <opencv4/opencv2/opencv.hpp>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "QRDetector.h"
#include "MovementDetector.h"
#include "YOLOv8Model.h"
#include <QMenu>
#include "VideoRecognizerConfiguration.h"
#include <QSettings>

namespace Ui {
class VideoRecognizer;
}

class VideoRecognizer : public QWidget {

    Q_OBJECT

public:

    explicit VideoRecognizer(QWidget *parent = nullptr);
    ~VideoRecognizer();

private slots:

    void displayDetectedQRs(QString QRDecoded, cv::Rect boundingRectangle);
    void displayDetectedMovement(std::vector<cv::Rect> boundingRectangles);
    void displayDetectedHazmats(std::vector<PredictionsData> predictions);

private:

    static const cv::Scalar QRRectangleColor;
    static const cv::Scalar MovementRectagleColor;
    static const cv::Scalar HazmatRectangleColor;

    Ui::VideoRecognizer* ui;

    QMenu* contextMenu_m;
    QAction* configurationAction_m;
    VideoRecognizerConfiguration* configurationDialog_m;

    QRDetector* QRDetector_m;
    MovementDetector* movementDetector_m;
    YOLOv8Model* hazmatDetector_m;

    QSettings settings_m;

    void detectQRIfChecked(cv::Mat frame);
    void detectMovementIfChecked(cv::Mat frame);
    void detectHazmatIfChecked(cv::Mat frame);

    void showContextMenu(const QPoint& mousePosition);
    void showConfigurationDialog();

    void applyConfigurationChanges();
    void loadConfigurations();
    void saveConfigurations();
};

#endif // VIDEO_RECOGNIZER_HEADER
