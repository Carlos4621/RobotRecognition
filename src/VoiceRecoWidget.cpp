#include "VoiceRecoWidget.h"

namespace My {

VoiceRecoWidget::VoiceRecoWidget(const QAudioDevice &device, const QAudioFormat &format, std::string_view voiceModelPath, QWidget *parent)
    : QWidget{ parent }
    , recordButton_m{ new QPushButton{tr("Record"), this} }
    , stopButton_m{ new QPushButton{tr("Stop"), this} }
    , voiceRecorder_m{ new My::VoiceRecorder{device, format, this} }
    , textEdit_m{ new QTextEdit{this} }
    , susurrador_m{ new Susurrador{voiceModelPath, this} }
    , transcription_m{ new QFutureWatcher<QString>{this} }
{
    connect(recordButton_m, &QPushButton::pressed, this, &VoiceRecoWidget::onRecordPressed);
    connect(stopButton_m, &QPushButton::pressed, this, &VoiceRecoWidget::onStopPressed);
    connect(transcription_m, &QFutureWatcher<QString>::finished, this, &VoiceRecoWidget::onAsyncFinish);

    setupLayout();
}

void VoiceRecoWidget::onRecordPressed() {
    recordButton_m->setEnabled(false);
    stopButton_m->setEnabled(true);

    textEdit_m->clear();

    voiceRecorder_m->start();
}

void VoiceRecoWidget::onStopPressed() {
    stopButton_m->setEnabled(false);

    voiceRecorder_m->stop();

    transcription_m->setFuture(QtConcurrent::run(&Susurrador::voiceToString, susurrador_m, voiceRecorder_m->getBuffer().buffer()));

    // Iniciar animación de carga
}

void VoiceRecoWidget::onAsyncFinish() {
    recordButton_m->setEnabled(true);

    textEdit_m->setText(transcription_m->result());

    // Terminar animación de carga
}

void VoiceRecoWidget::setupLayout() {

    QGridLayout* gridLayout{ new QGridLayout{this} };

    gridLayout->setHorizontalSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    gridLayout->addWidget(recordButton_m, 2, 0, 1, 1);

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(textEdit_m->sizePolicy().hasHeightForWidth());
    textEdit_m->setSizePolicy(sizePolicy);
    textEdit_m->setReadOnly(true);

    gridLayout->addWidget(textEdit_m, 0, 0, 1, 3);

    gridLayout->addWidget(stopButton_m, 2, 2, 1, 1);

    gridLayout->setColumnStretch(0, 5);
    gridLayout->setColumnStretch(2, 5);

    stopButton_m->setEnabled(false);
}

} // namespace My
