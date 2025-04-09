#include "VideoRecognizer.h"
#include "./ui_VideoRecognizer.h"

const cv::Scalar VideoRecognizer::QRRectangleColor{ 255, 0 ,255 };
const cv::Scalar VideoRecognizer::MovementRectagleColor{ 255, 255, 0 };
const cv::Scalar VideoRecognizer::HazmatRectangleColor{ 0, 255, 255};

VideoRecognizer::VideoRecognizer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoRecognizer)
    , QRDetector_m{ new QRDetector{this} }
    , movementDetector_m{ new MovementDetector{this} }
    , hazmatDetector_m{ new YOLOv8Model{this} }
{
    ui->setupUi(this);

    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectQRIfChecked);
    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectMovementIfChecked);
    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectHazmatIfChecked);

    connect(ui->detectMovement, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectMovementCheckboxChanged);
    connect(ui->detectQR, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectQRCheckboxChanged);
    connect(ui->detectHazmat, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectHazmatCheckboxChanged);

    connect(QRDetector_m, &QRDetector::QRDecoded, this, &VideoRecognizer::displayDetectedQRs);
    connect(movementDetector_m, &MovementDetector::movementDetected, this, &VideoRecognizer::displayDetectedMovement);
    connect(hazmatDetector_m, &YOLOv8Model::predictionsCompleted, this, &VideoRecognizer::displayDetectedHazmars);

    hazmatDetector_m->loadOnnxNetwork("/home/carlos4621/Robot/resources/hazmatModel.onnx", cv::Size(640, 640), false);
    hazmatDetector_m->loadClasses("/home/carlos4621/Robot/resources/hazmatClasses.txt");

    ui->cameraLabel->setCameraID(0);
    ui->cameraLabel->setFPS(13);
    ui->cameraLabel->startCamera();
}

VideoRecognizer::~VideoRecognizer() {
    delete ui;
}

void VideoRecognizer::displayDetectedQRs(QString QRDecoded, cv::Rect boundingRectangle) {
    if (QRDecoded.isEmpty()) {
        return;
    }

    ui->QROutput->setText(QRDecoded);
    ui->cameraLabel->drawRectangle(boundingRectangle.tl(), boundingRectangle.br(), QRRectangleColor);
}

void VideoRecognizer::displayDetectedMovement(std::vector<cv::Rect> boundingRectangles) {
    for (const auto& i : boundingRectangles) {
        ui->cameraLabel->drawRectangle(i.tl(), i.br(), MovementRectagleColor);
    }

    ui->movementOutput->setText("Detectado!");
}

void VideoRecognizer::displayDetectedHazmars(std::vector<PredictionsData> predictions) {
    ui->hazmatOutput->setText(QString::fromStdString(predictions[0].className));
}

void VideoRecognizer::onDetectMovementCheckboxChanged(const Qt::CheckState &newState) {
    if (newState == Qt::Unchecked) {
        ui->movementOutput->clear();
    }
}

void VideoRecognizer::onDetectQRCheckboxChanged(const Qt::CheckState &newState) {
    if (newState == Qt::Unchecked) {
        ui->QROutput->clear();
    }
}

void VideoRecognizer::onDetectHazmatCheckboxChanged(const Qt::CheckState &newState) {
    if (newState == Qt::Unchecked) {
        ui->hazmatOutput->clear();
    }
}

void VideoRecognizer::detectQRIfChecked(cv::Mat frame) {
    if (ui->detectQR->isChecked()) {
        QRDetector_m->detectQRsOnMat(frame);
    }
}

void VideoRecognizer::detectMovementIfChecked(cv::Mat frame) {
    if (ui->detectMovement->isChecked()) {
        ui->movementOutput->clear();
        movementDetector_m->addImage(frame);
    }
}

void VideoRecognizer::detectHazmatIfChecked(cv::Mat frame) {
    if (ui->detectHazmat->isChecked()) {
        hazmatDetector_m->predictOnMat(frame);
    }
}
