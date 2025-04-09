#pragma once
#ifndef BUFFERVOICERECORDER_H
#define BUFFERVOICERECORDER_H

#include <QObject>
#include <QtMultimedia>

namespace My {

/// @brief Clase que permite grabar audio en forma de QByteArray para su posterior procesamiento
class VoiceRecorder : public QObject {
    Q_OBJECT
public:

    /// @brief Constructor base
    /// @param device Dispositivo por el cual se grabará el audio
    /// @param format Formato en el que se registrará el audio
    /// @param parent Padre del widget
    VoiceRecorder(const QAudioDevice& device, const QAudioFormat& format, QObject *parent = nullptr);

    /// @brief Obtiene el buffer del audio grabado, debe estar detenida la grabación para su correcto uso
    /// @return Buffer con los datos del audio
    [[nodiscard]] const QBuffer& getBuffer() const noexcept;

public slots:

    /// @brief Inicia la grabación del audio
    void start();

    /// @brief Termina la grabación del audio
    void stop();

signals:

    void recordingStarted();
    void recordingStopped();

private:

    QAudioSource* source_m{ nullptr };
    QBuffer buffer_m;

    bool isRecording{ false };

    void setupSource();
};

} // namespace My

#endif // BUFFERVOICERECORDER_H
