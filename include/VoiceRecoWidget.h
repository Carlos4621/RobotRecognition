#pragma once
#ifndef VOICERECOWIDGET_H
#define VOICERECOWIDGET_H

#include <QWidget>
#include <QtMultimedia>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include "VoiceRecorder.h"
#include <QtConcurrent>
#include "Susurrador.h"

namespace My {

/// @brief Clase que representa un widget con opciones de transcripción de voz
class VoiceRecoWidget : public QWidget {
    Q_OBJECT
public:

    /// @brief Constructor base
    /// @param device Dispositivo por donde se grabará el audio
    /// @param format Formato en el que se grabará el audio, debe ser compatible con los estándares de Whisper.cpp
    /// @param voiceModelPath Path al modelo de voz a utilizar
    /// @param parent Padre del widget
    VoiceRecoWidget(const QAudioDevice& device, const QAudioFormat& format, std::string_view voiceModelPath, QWidget *parent = nullptr);

private slots:

    void onRecordPressed();
    void onStopPressed();
    void onAsyncFinish();

private:

    QTextEdit* textEdit_m{ nullptr };

    Susurrador* susurrador_m{ nullptr };

    My::VoiceRecorder* voiceRecorder_m{ nullptr };

    QPushButton* recordButton_m{ nullptr };
    QPushButton* stopButton_m{ nullptr };

    QFutureWatcher<QString>* transcription_m;

    void setupLayout();
};

} // namespace My

#endif // VOICERECOWIDGET_H
