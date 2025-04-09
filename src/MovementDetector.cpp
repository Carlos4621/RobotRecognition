#include "MovementDetector.h"

MovementDetector::MovementDetector(QObject *parent)
    : QObject{parent}
    , movementDetectorWorker_m{ new QFutureWatcher<std::vector<cv::Rect>>{this} }
    , backgroundSubstractor_m{ cv::createBackgroundSubtractorMOG2(500, 16, false) }
{
    connect(movementDetectorWorker_m, &QFutureWatcher<std::vector<cv::Rect>>::finished, this, &MovementDetector::emitIfMovementDetected);
}

void MovementDetector::addImage(const cv::Mat &image) {
    if (movementDetectorWorker_m->isRunning() || image.empty()) {
        return;
    }

    startWorker(image);
}

void MovementDetector::emitIfMovementDetected() {
    const auto results{ movementDetectorWorker_m->result() };

    if (!results.empty()) {
        emit movementDetected(results);
    }
}

void MovementDetector::startWorker(cv::Mat image) {
    movementDetectorWorker_m->setFuture(QtConcurrent::run(
        [this](cv::Mat image)
        {
            backgroundSubstractor_m->apply(image, image);

            cv::erode(image, image, {});
            cv::dilate(image, image, {});

            cv::threshold(image, image, 50, 255, cv::THRESH_BINARY);

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            std::vector<cv::Rect> rectangles;
            for (const auto& i : contours) {
                if (cv::contourArea(i) > RectangleMinSize) {
                    rectangles.emplace_back(cv::boundingRect(i));
                }
            }

            return rectangles;
        }
        , image));
}
