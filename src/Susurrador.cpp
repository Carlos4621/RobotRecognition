#include "Susurrador.h"

My::Susurrador::Susurrador(std::string_view modelPath, QObject *parent)
    : QObject{parent}
    , contextParams_m{ whisper_context_default_params() }
    , whisperParams_m{ whisper_full_default_params(WHISPER_SAMPLING_GREEDY)  }
    , context_m{ whisper_init_from_file_with_params(modelPath.data(), contextParams_m) }
{
}

My::Susurrador::~Susurrador() noexcept {
    whisper_free(context_m);
}

QString My::Susurrador::voiceToString(const QByteArray& buffer) {
    QString output;

    if (whisper_full(context_m, whisperParams_m, reinterpret_cast<const float*>(buffer.constData()), buffer.size() / sizeof(float)) == 0) {
        for (size_t i{ 0 }; i < whisper_full_n_segments(context_m); ++i) {
            output += whisper_full_get_segment_text(context_m, i);
        }
    }

    return output;
}
