#ifndef MOVEMENT_DETECTOR_HEADER
#define MOVEMENT_DETECTOR_HEADER

#include <QObject>
#include <opencv4/opencv2/opencv.hpp>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "UIUtils.h"
#include <span>

/// @brief Clase capaz de detectar movimiento en una serie de Mat
class MovementDetector : public QObject {

    Q_OBJECT

public:

    /// @brief MovementDetector Constructor base
    /// @param parent Padre del widget
    explicit MovementDetector(QObject *parent = nullptr);

    /// @brief addImage Añade una imagen para la detección de movimiento, en caso de detectarlo emite movementDetected
    /// @param image Imagen a añadir
    void addImage(const cv::Mat& image);

    /// @brief setHistory Cambia el número de frames a tener en cuenta para la detección de movimiento
    /// @param history Número de frames a tener en cuenta
    void setHistory(int history);

    /// @brief Obtiene el número de frames a tener en cuenta para la detección de movimiento
    /// @return Número de frames a tener en cuenta
    [[nodiscard]]
    int getHistory() const noexcept;

    /// @brief setThreshold Cambia el umbral de diferencia entre frames para la detección de movimiento
    /// @param threshold Umbral de diferencia entre frames
    void setThreshold(double threshold);

    /// @brief Obtiene el umbral de diferencia entre frames para la detección de movimiento
    /// @return El umbral de diferencia entre frames
    [[nodiscard]]
    double getThreshold() const noexcept;

    /// @brief setDetectShadows Cambia si se detectan sombras o no
    /// @param detectShadows Si se detectan sombras o no
    void setDetectShadows(bool detectShadows);

    /// @brief Obtiene si se detectan sombras o no
    /// @return El valor de si se detectan sombras o no
    [[nodiscard]]
    bool getDetectShadows() const noexcept;

    /// @brief setRectangleMinSize Cambia el tamaño mínimo de los cuadros contenedores del movimiento
    /// @param size Tamaño mínimo de los cuadros contenedores del movimiento
    void setRectangleMinSize(size_t size);

    /// @brief Obtiene el tamaño mínimo de los cuadros contenedores del movimiento
    /// @return El tamaño mínimo de los cuadros contenedores del movimiento
    [[nodiscard]]
    size_t getRectangleMinSize() const noexcept;

    /// @brief setDetectionThreshold Cambia el umbral de detección de movimiento
    /// @param threshold Umbral de detección de movimiento
    void setDetectionThreshold(int threshold);

    /// @brief Obtiene el umbral de detección de movimiento
    /// @return El umbral de detección de movimiento
    [[nodiscard]]
    int getDetectionThreshold() const noexcept;

signals:

    /// @brief movementDetected Emitido cada vez que se detecta movimiento
    /// @param boundingRect Cuadros contenedores del movimiento
    void movementDetected(const std::vector<cv::Rect>& boundingRects);

private slots:

    void emitIfMovementDetected();

private:

    static constexpr int MaxPixelValue{ 255 };

    QFutureWatcher<std::vector<cv::Rect>> movementDetectorWorker_m;

    cv::Ptr<cv::BackgroundSubtractor> backgroundSubstractor_m;

    cv::Mat morphKernel_m;

    int history_m{ 500 };
    double threshold_m{ 16 };
    bool detectShadows_m{ false };
    bool changesToApplyToBackgroundSubstractor_m{ false };

    size_t detectionRectangleMinimumSize_m{ 5000 };
    int detectionThreshold_m{ 50 };

    void startWorker(const cv::Mat& image);

    void applyChangesIfRequired();

    std::vector<cv::Rect> processImage(cv::Mat workingImage);
};

#endif // MOVEMENT_DETECTOR_HEADER