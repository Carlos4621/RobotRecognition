#ifndef MATLABEL_H
#define MATLABEL_H

#include <QLabel>
#include <opencv4/opencv2/opencv.hpp>

/// @brief QLabel con capacidad de mostrar cv::Mat
class MatLabel : public QLabel {

    Q_OBJECT

public:

    /// @brief Constructor base
    /// @param parent Padre del widget
    explicit MatLabel(QWidget* parent = nullptr);

    /// @brief setMat Muestra el Mat en el QLabel
    /// @param mat Mat a mostrar
    void setMat(const cv::Mat &mat);

    /// @brief getMat Obtiene el Mat mostrado
    /// @return Mat mostrado
    [[nodiscard]]
    cv::Mat getMat() const noexcept;

signals:

    /// @brief matChanged Señal emitida cuando se cambia el Mat mostrado
    /// @param newMat Nuevo Mat
    void matChanged(cv::Mat newMat);

private:

    cv::Mat currentMat_m;

    [[nodiscard]]
    static QPixmap matToPixmap(const cv::Mat& toConvert) noexcept;
};

#endif // MATLABEL_H
