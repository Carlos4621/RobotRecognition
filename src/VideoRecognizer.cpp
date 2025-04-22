#include "VideoRecognizer.h"
#include "./ui_VideoRecognizer.h"

const cv::Scalar VideoRecognizer::QRRectangleColor{ 255, 0 ,255 };
const cv::Scalar VideoRecognizer::MovementRectagleColor{ 255, 255, 0 };
const cv::Scalar VideoRecognizer::HazmatRectangleColor{ 0, 255, 255};

VideoRecognizer::VideoRecognizer(QWidget *parent)
    : QWidget{ parent }
    , ui(new Ui::VideoRecognizer)
    , QRDetector_m{ new QRDetector{this} }
    , movementDetector_m{ new MovementDetector{this} }
    , hazmatDetector_m{ new YOLOv8Model{this} }
    , contextMenu_m{ new QMenu{this} }
    , configurationAction_m{ new QAction{tr("Configuration"), this} }
    , configurationDialog_m{ new VideoRecognizerConfiguration{ this } }
    , settings_m{ "ControladorRobot", "VideoRecognizer" }
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::CustomContextMenu);

    contextMenu_m->addAction(configurationAction_m);

    connect(this, &VideoRecognizer::customContextMenuRequested, this, &VideoRecognizer::showContextMenu);
    connect(configurationAction_m, &QAction::triggered, this, &VideoRecognizer::showConfigurationDialog);

    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectQRIfChecked);
    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectMovementIfChecked);
    connect(ui->cameraLabel, &CameraLabel::frameChanged, this, &VideoRecognizer::detectHazmatIfChecked);

    connect(ui->detectMovement, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectMovementCheckboxChanged);
    connect(ui->detectQR, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectQRCheckboxChanged);
    connect(ui->detectHazmat, &QCheckBox::checkStateChanged, this, &VideoRecognizer::onDetectHazmatCheckboxChanged);

    connect(QRDetector_m, &QRDetector::QRDecoded, this, &VideoRecognizer::displayDetectedQRs);
    connect(movementDetector_m, &MovementDetector::movementDetected, this, &VideoRecognizer::displayDetectedMovement);
    connect(hazmatDetector_m, &YOLOv8Model::predictionsCompleted, this, &VideoRecognizer::displayDetectedHazmars);

    loadConfigurations();

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

void VideoRecognizer::showContextMenu(const QPoint &mousePosition) {
    contextMenu_m->exec(mapToGlobal(mousePosition));
}

void VideoRecognizer::showConfigurationDialog() {
    configurationDialog_m->exec();

    if (configurationDialog_m->result() == QDialog::Accepted) {
        applyConfigurationChanges();
        saveConfigurations();
    } else {
        configurationDialog_m->loadConfigurations();
    }
}

void VideoRecognizer::applyConfigurationChanges() {
    ui->cameraLabel->setCameraID(configurationDialog_m->getCameraID());
    ui->cameraLabel->setFPS(configurationDialog_m->getFPS());
    ui->cameraLabel->startCamera();
    hazmatDetector_m->loadOnnxNetwork(configurationDialog_m->getONNXPath(), {640, 640}, false);
    hazmatDetector_m->loadClasses(configurationDialog_m->getClassesPath());
}

void VideoRecognizer::loadConfigurations() {
    settings_m.beginGroup("VideoRecognizer");
    ui->cameraLabel->setCameraID(settings_m.value("cameraID", 0).toUInt());
    ui->cameraLabel->setFPS(settings_m.value("fps", 15).toFloat());
    hazmatDetector_m->loadOnnxNetwork(settings_m.value("onnxPath", "~").toString().toStdString(), {640, 640}, false);
    hazmatDetector_m->loadClasses(settings_m.value("classesPath", "~").toString().toStdString());
    settings_m.endGroup();
}

void VideoRecognizer::saveConfigurations() {
    settings_m.beginGroup("VideoRecognizer");
    settings_m.setValue("cameraID", ui->cameraLabel->getCameraID());
    settings_m.setValue("fps", ui->cameraLabel->getFPS());
    settings_m.setValue("onnxPath", QString::fromStdString(hazmatDetector_m->getOnnxPath()));
    settings_m.setValue("classesPath", QString::fromStdString(hazmatDetector_m->getClassesPath()));
    settings_m.endGroup();
}
