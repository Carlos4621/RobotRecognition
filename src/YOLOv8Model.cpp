#include "YOLOv8Model.h"
#include <fstream>

YOLOv8Model::YOLOv8Model(QObject *parent)
    : QObject{parent}
    , predicterWorker_m{ new QFutureWatcher<std::vector<PredictionsData>>{this} }
{
    connect(predicterWorker_m, &QFutureWatcher<std::vector<PredictionsData>>::finished, this, &YOLOv8Model::emitIfPredicted);
}

void YOLOv8Model::predictOnMat(const cv::Mat& inputImage, double modelConfidenceThreshold, double modelScoreThreshold, double modelNMSThreshold) {
    if (predicterWorker_m->isRunning() || inputImage.empty()) {
        return;
    }

    verifyModelLoaded();

    startWorker(inputImage, modelConfidenceThreshold, modelScoreThreshold, modelNMSThreshold);
}

void YOLOv8Model::emitIfPredicted() {
    const auto results{ predicterWorker_m->result() };

    if (!results.empty()) {
        emit predictionsCompleted(results);
    }
}

void YOLOv8Model::startWorker(const cv::Mat &inputImage, double modelConfidenceThreshold, double modelScoreThreshold, double modelNMSThreshold) {
    predicterWorker_m->setFuture(QtConcurrent::run(
        [this, inputImage, modelConfidenceThreshold, modelScoreThreshold, modelNMSThreshold] {
            cv::Mat blob;
            cv::dnn::blobFromImage(inputImage, blob, 1.0/255.0, modelInputSize_m, cv::Scalar{}, true);
            network_m.setInput(blob);

            std::vector<cv::Mat> networkOutputs;
            network_m.forward(networkOutputs);

            const auto rows{ networkOutputs[0].size[2] };
            const auto dimensions{ networkOutputs[0].size[1] };

            networkOutputs[0] = networkOutputs[0].reshape(1, dimensions);
            cv::transpose(networkOutputs[0], networkOutputs[0]);

            auto data{ reinterpret_cast<float*>(networkOutputs[0].data) };

            const auto Xfactor{ inputImage.cols / modelInputSize_m.width };
            const auto Yfactor{ inputImage.rows / modelInputSize_m.height };

            std::vector<uint8_t> classIDs;
            std::vector<float> confidences;
            std::vector<cv::Rect> boxes;

            for (size_t i{ 0 }; i < rows; ++i) {

                auto const classes_scores{ data + 4 };

                cv::Mat scores(1, classNames_m.size(), CV_32FC1, classes_scores);
                cv::Point classID;
                double maxClassScore;

                cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classID);

                if (maxClassScore > modelScoreThreshold) {

                    confidences.push_back(maxClassScore);
                    classIDs.push_back(classID.x);

                    const auto& x{ data[0] };
                    const auto& y{ data[1] };
                    const auto& w{ data[2] };
                    const auto& h{ data[3] };

                    const auto left{ static_cast<int>((x - (0.5 * w)) * Xfactor) };
                    const auto top{ static_cast<int>((y - (0.5 * h)) * Yfactor) };

                    const auto width{ static_cast<int>(w * Xfactor) };
                    const auto height{ static_cast<int>(h * Yfactor) };

                    boxes.emplace_back(left, top, width, height);
                }

                data += dimensions;
            }

            std::vector<int> NMSResult;
            cv::dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, NMSResult);

            std::vector<PredictionsData> inferences;
            inferences.reserve(NMSResult.size());

            for (const auto &i : NMSResult) {
                inferences.emplace_back(classIDs[i], classNames_m[classIDs[i]], confidences[i], boxes[i]);
            }

            return inferences;
        }));
}

void YOLOv8Model::verifyModelLoaded() {
    if (network_m.empty()) {
        UiUtils::showError("El modelo ONNX no está cargado");
        return;
    }
    
    if (classNames_m.empty()) {
        UiUtils::showError("Las clases no están cargadas");
        return;
    }
}

void YOLOv8Model::loadClasses(std::string_view path) {
    std::ifstream inputFile(path.data());

    if (inputFile.is_open()) {

        std::string classLine;

        while (std::getline(inputFile, classLine)) {
            classNames_m.push_back(classLine);
        }

        classesPath_m = path.data();

    } 
    else {
        UiUtils::showError("Unable to load class names file with path: " + QString::fromUtf8(path.data()));
    }
}

std::string YOLOv8Model::getClassesPath() const noexcept {
    return classesPath_m;
}

void YOLOv8Model::loadOnnxNetwork(std::string_view path, const cv::Size2f& modelInputSize, bool cudaEnabled) {
    modelInputSize_m = modelInputSize;

    try {
        network_m = cv::dnn::readNetFromONNX(path.data());

        if (cudaEnabled) {
            network_m.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            network_m.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
        }
        else {
            network_m.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            network_m.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }

        onnxPath_m = path.data();

    } catch (const cv::Exception& e) {
        UiUtils::showError("Unable to load ONNX model with path: " + QString::fromUtf8(path.data()) + "\n" + QString::fromStdString(e.what()));
    }
}

std::string YOLOv8Model::getOnnxPath() const noexcept {
    return onnxPath_m;
}
