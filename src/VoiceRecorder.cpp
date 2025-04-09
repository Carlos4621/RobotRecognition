#include "VoiceRecorder.h"

namespace My {

VoiceRecorder::VoiceRecorder(const QAudioDevice &device, const QAudioFormat &format, QObject *parent)
    : QObject{ parent }
{
    if (!device.isFormatSupported(format)) {
        throw std::runtime_error{ "Format not supported in the device selected" };
    }

    source_m = new QAudioSource{ device, format };
}

void VoiceRecorder::start() {
    if (isRecording) {
        return;
    }

    buffer_m.reset();
    buffer_m.open(QIODevice::WriteOnly);
    source_m->start(&buffer_m);

    isRecording = true;

    emit recordingStarted();
}

void VoiceRecorder::stop() {
    if (!isRecording) {
        return;
    }

    buffer_m.close();
    source_m->stop();

    isRecording = false;

    emit recordingStopped();
}

const QBuffer& VoiceRecorder::getBuffer() const noexcept {
    return buffer_m;
}

} // namespace My
