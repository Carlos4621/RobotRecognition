#ifndef MAT_LABEL_HEADER
#define MAT_LABEL_HEADER

#include <QLabel>
#include <opencv4/opencv2/opencv.hpp>
#include "UIUtils.h"

/// @brief QLabel con capacidad de mostrar cv::Mat
class MatLabel : public QLabel {

    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit MatLabel(QWidget* parent = nullptr);

    /// @brief setMat Muestra el Mat en el QLabel
    /// @param mat Mat a mostrar
    void setMat(const cv::Mat& newMat);

    /// @brief getMat Obtiene el Mat mostrado
    /// @return Mat mostrado
    [[nodiscard]]
    cv::Mat getMat() const noexcept;

signals:

    /// @brief matChanged Señal emitida cuando se cambia el Mat mostrado
    /// @param newMat Nuevo Mat
    void matChanged(const cv::Mat& newMat);

private:

    cv::Mat currentMat_m;

    [[nodiscard]]
    static QPixmap matToPixmap(const cv::Mat& toConvert) noexcept;
};

#endif // MAT_LABEL_HEADER
