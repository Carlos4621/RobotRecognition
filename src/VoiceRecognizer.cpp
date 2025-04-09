#include "VoiceRecognizer.h"

namespace My {

VoiceRecognizer::VoiceRecognizer(const QAudioDevice &device, const QAudioFormat &format, std::string_view voiceModelPath, QWidget *parent)
    : QWidget{ parent }
    , recordButton_m{ new QPushButton{tr("Record"), this} }
    , stopButton_m{ new QPushButton{tr("Stop"), this} }
    , voiceRecorder_m{ new My::VoiceRecorder{device, format, this} }
    , textEdit_m{ new QTextEdit{this} }
    , susurrador_m{ new Susurrador{voiceModelPath, this} }
    , transcription_m{ new QFutureWatcher<QString>{this} }
{
    connect(recordButton_m, &QPushButton::pressed, this, &VoiceRecognizer::onRecordPressed);
    connect(stopButton_m, &QPushButton::pressed, this, &VoiceRecognizer::onStopPressed);
    connect(transcription_m, &QFutureWatcher<QString>::finished, this, &VoiceRecognizer::onAsyncFinish);
}

void VoiceRecognizer::onRecordPressed() {
    recordButton_m->setEnabled(false);
    stopButton_m->setEnabled(true);

    textEdit_m->clear();

    voiceRecorder_m->start();
}

void VoiceRecognizer::onStopPressed() {
    stopButton_m->setEnabled(false);

    voiceRecorder_m->stop();

    transcription_m->setFuture(QtConcurrent::run(&Susurrador::voiceToString, susurrador_m, voiceRecorder_m->getBuffer().buffer()));

    // Iniciar animación de carga
}

void VoiceRecognizer::onAsyncFinish() {
    recordButton_m->setEnabled(true);

    textEdit_m->setText(transcription_m->result());

    // Terminar animación de carga
}

} // namespace My
