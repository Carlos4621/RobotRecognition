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
        emit errorOcurred(QString::fromUtf8("Invalid FPS: ") + QString::number(targetFPS));
        return;
    }

    cameraTimer_m->setInterval(FPStoInterval(targetFPS));

    emit FPSChanged(targetFPS);
}

void CameraLabel::setCameraID(uint8_t newID) {
    camera_m.open(newID);

    if (!camera_m.isOpened()) {
        emit errorOcurred(QString::fromUtf8("Camera not opened with ID: ") + QString::number(newID));
        return;
    }

    emit cameraIDChanged(newID);
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
        emit errorOcurred(QString::fromUtf8("Empty frame"));
        return;
    }

    matLabel_m->setMat(nextFrame);

    emit frameChanged(nextFrame);
}

constexpr float CameraLabel::FPStoInterval(float FPS) noexcept {
    return 1000.f / FPS;
}
