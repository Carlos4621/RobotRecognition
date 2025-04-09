#include "QRDetector.h"

QRDetector::QRDetector(QWidget *parent)
    : QObject{ parent }
    , QRDetectorWorker_m{ new QFutureWatcher<QRData>{this} }
{
    connect(QRDetectorWorker_m, &QFutureWatcher<QRData>::finished, this, &QRDetector::emitIfQRFounded);
}

void QRDetector::detectQRsOnMat(const cv::Mat& image) {
    if (QRDetectorWorker_m->isRunning() || image.empty()) {
        return;
    }

    startQRWorker(image);
}

void QRDetector::startQRWorker(const cv::Mat& originalImage) {
    QRDetectorWorker_m->setFuture(QtConcurrent::run(
        [this](cv::Mat image)
        {
            cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            cv::equalizeHist(image, image);

            std::vector<cv::Point> points;
            const auto decodedQR{ QRDetector_m.detectAndDecode(image, points) };

            const auto text = QString::fromStdString(decodedQR);

            return QRData{ text, cv::boundingRect(points) };
        }
        , originalImage));
}

void QRDetector::emitIfQRFounded() {
    const auto decodedQRData{ QRDetectorWorker_m->result() };

    if (!decodedQRData.second.empty()) {
        emit QRDecoded(decodedQRData.first, decodedQRData.second);
    }
}
