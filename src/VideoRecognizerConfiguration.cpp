#include "VideoRecognizerConfiguration.h"
#include "./ui_VideoRecognizerConfiguration.h"

VideoRecognizerConfiguration::VideoRecognizerConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoRecognizerConfiguration),
    settings_m{ "ControladorRobot", "VideoRecognizer" }
{
    ui->setupUi(this);
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

void VideoRecognizerConfiguration::loadConfigurations() {
    settings_m.beginGroup("VideoRecognizer");
    ui->cameraID->setText(settings_m.value("cameraID", 0).toString());
    ui->maxFPS->setValue(settings_m.value("fps", 15).toFloat());
    ui->modelPath->setText(settings_m.value("onnxPath", "~").toString());
    ui->modelClassesPath->setText(settings_m.value("classesPath", "~").toString());
    settings_m.endGroup();
}
