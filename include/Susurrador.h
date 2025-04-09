#pragma once
#ifndef SUSURRADOR_H
#define SUSURRADOR_H

#include <whisper.h>
#include <string_view>
#include <QBuffer>

namespace My {

/// @brief Clase que implementa el C-Api de Whisper.cpp para su uso orientado a objetos y su integración a Qt Multimedia
class Susurrador : public QObject {
    Q_OBJECT
public:

    /// @brief Constructor base
    /// @param modelPath Path del modelo a usar
    explicit Susurrador(std::string_view modelPath, QObject* parent = nullptr);

    virtual ~Susurrador() noexcept;

    /// @brief Transcribe el texto enviado desde un QByteArray
    /// @param buffer Audio a transcribir
    [[nodiscard]] QString voiceToString(const QByteArray& buffer);

private:

    whisper_context_params contextParams_m;
    whisper_full_params whisperParams_m;
    whisper_context* context_m;
};

}

#endif // SUSURRADOR_H
