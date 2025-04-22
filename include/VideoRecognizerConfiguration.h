#ifndef VIDEO_RECOGNIZER_CONFIGURATION_HEADER
#define VIDEO_RECOGNIZER_CONFIGURATION_HEADER

#include <QWidget>
#include <QDialog>
#include <QSettings>

namespace Ui {
class VideoRecognizerConfiguration;
}

/// @brief Clase encargada de mostrar la configuración del video
class VideoRecognizerConfiguration : public QDialog {

    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit VideoRecognizerConfiguration(QWidget *parent = nullptr);

    ~VideoRecognizerConfiguration();

    /// @brief Devuelve el valor dentro del campo cameraID
    /// @return Valor dentro del campo cameraID
    [[nodiscard]]
    uint8_t getCameraID() const noexcept;

    /// @brief Devuelve el valor dentro del campo maxFPS
    /// @return Valor dentro del campo maxFPS
    [[nodiscard]]
    float getFPS() const noexcept;

    /// @brief Devuelve el valor dentro del campo modelPath
    /// @return Valor dentro del campo modelPath
    [[nodiscard]]
    std::string getONNXPath() const noexcept;

    /// @brief Devuelve el valor dentro del campo modelClassesPath
    /// @return Valor dentro del campo modelClassesPath
    [[nodiscard]]
    std::string getClassesPath() const noexcept;

    /// @brief Carga la configuración guardada anteriormente
    void loadConfigurations();

private:

    Ui::VideoRecognizerConfiguration* ui;

    QSettings settings_m;

};

#endif // !VIDEO_RECOGNIZER_CONFIGURATION_HEADER