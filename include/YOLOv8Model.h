#ifndef YOLO_V8_MODEL_HEADER
#define YOLO_V8_MODEL_HEADER

#include <vector>
#include <string>
#include <string_view>
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

/// @brief Struct que contiene los datos de las predicciones
struct PredictionsData {
    uint16_t classID;
    std::string className;
    double confidence;
    cv::Rect bouncingBox;
};

/// @brief Clase que permite el uso de modelos YOLOv8 exportados a .onnx
class YOLOv8Model : public QObject {

    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del QObject
    explicit YOLOv8Model(QObject* parent = nullptr);

    /// @brief loadClasses Carga el archivo .txt que contiene los IDs de las predicciones
    /// @param path Ruta al archivo .txt
    void loadClasses(std::string_view path);

    /// @brief loadOnnxNetwork Carga el archivo .onnx que contiene el modelo a usar
    /// @param path Ruta al archivo .onnx
    /// @param modelInputSize Tamaño a redimensionar de la imagen antes de aplicarse el modelo
    /// @param cudaEnabled Activa CUDA
    void loadOnnxNetwork(std::string_view path, const cv::Size2f& modelInputSize, bool cudaEnabled);

    /// @brief Aplica el modelo a la imagen proporcionada y emite predictionsCompleted cuando termina
    /// @param inputImage Imagen a usar
    /// @param modelConfidenceThreshold Threshold de la confidencia. Default = 0.25
    /// @param modelScoreThreshold Threshold del score. Default = 0.45
    /// @param modelNMSThreshold Threshold del NMS. Default = 0.5
    void predictOnMat(const cv::Mat& inputImage, double modelConfidenceThreshold = 0.25f, double modelScoreThreshold = 0.45f, double modelNMSThreshold = 0.5f);

signals:

    /// @brief predictionsCompleted Se emite cada vez que se termina de predecir
    /// @param predictionsData Datos de la predicción
    void predictionsCompleted(std::vector<PredictionsData> predictionsData);

private slots:

    void emitIfPredicted();

private:

    QFutureWatcher<std::vector<PredictionsData>>* predicterWorker_m;

    std::vector<std::string> classNames_m;

    cv::Size2f modelInputSize_m{ 640, 640 };

    cv::dnn::Net network_m;

    void startWorker(const cv::Mat& inputImage, double modelConfidenceThreshold, double modelScoreThreshold, double modelNMSThreshold);
};

#endif // YOLO_V8_MODEL_HEADER
