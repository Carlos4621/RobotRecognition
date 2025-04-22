#include "CameraLabel.h"

CameraLabel::CameraLabel(QWidget *parent)
    : QWidget{ parent }
    , cameraTimer_m{ new QTimer{this} }
    , matLabel_m{ new MatLabel{this} }
{
    QVBoxLayout *layout = new QVBoxLayout{this};
    layout->addWidget(matLabel_m);

    connect(cameraTimer_m, &QTimer::timeout, this, &CameraLabel::nextFrame);
}

void CameraLabel::setFPS(float targetFPS) {
    if (targetFPS <= 0) {
        UiUtils::showError("FPS inválidos: " + QString::number(targetFPS));
        return;
    }

    cameraTimer_m->setInterval(FPStoInterval(targetFPS));

    emit FPSChanged(targetFPS);
}

float CameraLabel::getFPS() const noexcept {
    return intervalToFPS(cameraTimer_m->interval());
}

void CameraLabel::setCameraID(uint8_t newID) {
    camera_m.open(newID);

    if (!camera_m.isOpened()) {
        UiUtils::showError("No se puede abrir la cámara con ID: " + QString::number(newID));
        stopCamera();
        return;
    }

    cameraID_m = newID;

    emit cameraIDChanged(newID);
}

uint8_t CameraLabel::getCameraID() const noexcept {
    return cameraID_m;
}

void CameraLabel::drawRectangle(const cv::Point &topLeft, const cv::Point &bottomRight, const cv::Scalar& color) {
    const auto actualFrame{ matLabel_m->getMat() };

    cv::rectangle(actualFrame, topLeft, bottomRight, color);

    matLabel_m->setMat(actualFrame);
}

void CameraLabel::startCamera() {
    cameraTimer_m->start();
}

void CameraLabel::stopCamera() {
    cameraTimer_m->stop();
}

void CameraLabel::nextFrame() {
    cv::Mat nextFrame;

    camera_m >> nextFrame;

    if (nextFrame.empty()) {
        UiUtils::showError("No se puede obtener el siguiente frame");
        stopCamera();
        return;
    }

    matLabel_m->setMat(nextFrame);

    emit frameChanged(nextFrame);
}

constexpr float CameraLabel::FPStoInterval(float FPS) noexcept {
    return 1000.f / FPS;
}

constexpr float CameraLabel::intervalToFPS(float interval) noexcept {
    return 1000.f / interval;
}
