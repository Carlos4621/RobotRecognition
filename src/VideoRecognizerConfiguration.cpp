#include "VideoRecognizerConfiguration.h"
#include "./ui_VideoRecognizerConfiguration.h"

VideoRecognizerConfiguration::VideoRecognizerConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoRecognizerConfiguration),
    settings_m{ "ControladorRobot", "VideoRecognizer" }
{
    ui->setupUi(this);
    ui->cameraID->setValidator(new QIntValidator{this});
    this->setFixedSize(this->size());
    loadConfigurations();
}

VideoRecognizerConfiguration::~VideoRecognizerConfiguration() {
    delete ui;
}

uint8_t VideoRecognizerConfiguration::getCameraID() const noexcept {
    return static_cast<uint8_t>(ui->cameraID->text().toUInt());
}

float VideoRecognizerConfiguration::getFPS() const noexcept {
    return ui->maxFPS->value();
}

std::string VideoRecognizerConfiguration::getONNXPath() const noexcept {
    return ui->modelPath->text().toStdString();
}

std::string VideoRecognizerConfiguration::getClassesPath() const noexcept {
    return ui->modelClassesPath->text().toStdString();
}

int VideoRecognizerConfiguration::getImageHistory() const noexcept {
    return ui->imageHistory->text().toInt();
}

int VideoRecognizerConfiguration::getRectangleMinSize() const noexcept {
    return ui->rectangleMinSize->text().toInt();
}

int VideoRecognizerConfiguration::getDetectionThreshold() const noexcept {
    return ui->framesThreshold->value();
}

double VideoRecognizerConfiguration::getMovementThreshold() const noexcept {
    return ui->movementThreshold->value();
}

bool VideoRecognizerConfiguration::getDetectShadows() const noexcept {
    return ui->detectShadows->isChecked();
}

void VideoRecognizerConfiguration::loadConfigurations() {
    settings_m.beginGroup("VideoRecognizer");
    ui->cameraID->setText(settings_m.value("cameraID", 0).toString());
    ui->maxFPS->setValue(settings_m.value("fps", 15).toFloat());
    ui->modelPath->setText(settings_m.value("onnxPath", "~").toString());
    ui->modelClassesPath->setText(settings_m.value("classesPath", "~").toString());
    ui->imageHistory->setText(settings_m.value("imageHistory", 500).toString());
    ui->rectangleMinSize->setText(settings_m.value("rectangleMinSize", 5000).toString());
    ui->framesThreshold->setValue(settings_m.value("detectionThreshold", 50).toInt());
    ui->movementThreshold->setValue(settings_m.value("movementThreshold", 16).toDouble());
    ui->detectShadows->setChecked(settings_m.value("detectShadows", false).toBool());
    settings_m.endGroup();
}
