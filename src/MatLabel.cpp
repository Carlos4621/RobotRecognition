#include "MatLabel.h"

MatLabel::MatLabel(QWidget *parent)
    : QLabel{ parent }
{
    setScaledContents(true);
    setMinimumSize(1, 1);
}

void MatLabel::setMat(const cv::Mat& mat) {
    currentMat_m = mat;

    this->setPixmap(matToPixmap(currentMat_m));

    emit matChanged(currentMat_m);
}

cv::Mat MatLabel::getMat() const noexcept {
    return currentMat_m;
}

QPixmap MatLabel::matToPixmap(const cv::Mat &toConvert) noexcept {
    if (toConvert.empty()) {
        return QPixmap{};
    }

    const QImage qtImage(reinterpret_cast<uint8_t*>(toConvert.data), toConvert.cols, toConvert.rows, toConvert.step, QImage::Format_BGR888);

    return QPixmap::fromImage(qtImage);
}
