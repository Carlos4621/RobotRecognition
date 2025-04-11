#ifndef CAMERA_LABEL_HEADER
#define CAMERA_LABEL_HEADER

#include <QLabel>
#include <QObject>
#include <QTimer>
#include "MatLabel.h"
#include <QLayout>
#include "UIUtils.h"

/// @brief Permite mostrar tu cámara en un MatLabel
class CameraLabel : public QWidget {

    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit CameraLabel(QWidget *parent = nullptr);

    /// @brief setFPS Cambia los fotogramas por segundo
    /// @param targetFPS FPS a establecer
    void setFPS(float targetFPS);

    /// @brief setCameraID Cambia el ID de la cámara a mostrar
    /// @param newID ID de la nueva cámara
    void setCameraID(uint8_t newID);

    /// @brief drawRectangle Dibuja un rectángulo en la posición deseada
    /// @param topLeft Punto de la esquina superior izquierda del rectángulo
    /// @param bottomRight Punto de la esquina inferior derecha del rectángulo
    /// @param color Color del rectángulo a dibujar
    void drawRectangle(const cv::Point& topLeft, const cv::Point& bottomRight,  const cv::Scalar& color);

public slots:

    /// @brief startCamera Inicia la cámara
    void startCamera();

    /// @brief stopCamera Detiene la cámara, no borra el frame actual
    void stopCamera();

    /// @brief nextFrame Obtiene el siguiente frame y lo muestra
    void nextFrame();

signals:

    /// @brief FPSChanged Emitida cuando se cambian los fps
    /// @param newFPS Nuevos FPS
    void FPSChanged(float newFPS);

    /// @brief cameraIDChanged Emitida cuando se cambia el ID de la cámara
    /// @param newCameraID Nuevo ID de la cámara
    void cameraIDChanged(uint8_t newCameraID);

    /// @brief frameChanged Emitido cuando la cámara muestra otro frame
    /// @param newFrame Nuevo frame mostrado
    void frameChanged(cv::Mat newFrame);

private:

    QTimer* cameraTimer_m;
    MatLabel* matLabel_m;

    cv::VideoCapture camera_m;

    [[nodiscard]]
    static constexpr float FPStoInterval(float FPS) noexcept;
};

#endif // CAMERA_LABEL_HEADER
