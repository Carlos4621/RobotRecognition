#include "MovementDetector.h"

MovementDetector::MovementDetector(QObject *parent)
    : QObject{parent}
    , backgroundSubstractor_m{ cv::createBackgroundSubtractorMOG2(history_m, threshold_m, detectShadows_m) }
{
    connect(&movementDetectorWorker_m, &QFutureWatcher<std::vector<cv::Rect>>::finished, this, &MovementDetector::emitIfMovementDetected);
}

void MovementDetector::addImage(const cv::Mat &image) {
    if (movementDetectorWorker_m.isRunning() || image.empty()) {
        return;
    }

    applyChangesIfRequired();

    startWorker(image);
}

void MovementDetector::setHistory(int history) {
    if (history <= 0) {
        UiUtils::showError("History must be greater than 0");
        return;
    }

    history_m = history;
    changesToApplyToBackgroundSubstractor_m = true;
}

void MovementDetector::setThreshold(double threshold) {
    if (threshold < 0) {
        UiUtils::showError("Threshold must be greater than or equal to 0");
        return;
    }

    threshold_m = threshold;
    changesToApplyToBackgroundSubstractor_m = true;
}

void MovementDetector::setDetectShadows(bool detectShadows) {
    detectShadows_m = detectShadows;
    changesToApplyToBackgroundSubstractor_m = true;
}

void MovementDetector::setRectangleMinSize(size_t size) {
    detectionRectangleMinimunSize_m = size;
}

void MovementDetector::setDetectionThreshold(int threshold) {
    if (threshold < 0 || threshold > MaxPixelValue) {
        UiUtils::showError("Detection threshold must be between 0 and " + QString::number(MaxPixelValue));
        return;
    }

    detectionThreshold_m = threshold;
}

void MovementDetector::applyChangesIfRequired() {
    if (changesToApplyToBackgroundSubstractor_m) {
        auto newBgSubtractor = cv::createBackgroundSubtractorMOG2(history_m, threshold_m, detectShadows_m);

        if (newBgSubtractor) {
            backgroundSubstractor_m = newBgSubtractor;
            
        } else {
            UiUtils::showError("Error creating background subtractor with params: " 
                               "history: " + QString::number(history_m) +
                               ", threshold: " + QString::number(threshold_m) +
                               ", detectShadows: " + QString::number(detectShadows_m));
        }

        changesToApplyToBackgroundSubstractor_m = false;
    }
}

std::vector<cv::Rect> MovementDetector::processImage(cv::Mat workingImage) {
    backgroundSubstractor_m->apply(workingImage, workingImage);

    cv::erode(workingImage, workingImage, {});
    cv::dilate(workingImage, workingImage, {});
    cv::threshold(workingImage, workingImage, detectionThreshold_m, MaxPixelValue, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(workingImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> rectangles;
    for (const auto& i : contours) {
        if (cv::contourArea(i) > detectionRectangleMinimunSize_m) {
            rectangles.emplace_back(cv::boundingRect(i));
        }
    }

    return rectangles;
}

void MovementDetector::emitIfMovementDetected() {
    const auto results{ movementDetectorWorker_m.result() };

    if (!results.empty()) {
        emit movementDetected(results);
    }
}

void MovementDetector::startWorker(const cv::Mat& image) {
    cv::Mat imageCopy = image.clone();
    movementDetectorWorker_m.setFuture(QtConcurrent::run(
        [this, imageCopy]() {
            return this->processImage(imageCopy);
        }
    ));
}
