#pragma once
#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QObject>

class ImageLabel : public QLabel {
public:

    QSize sizeHint() const override;
};

#endif // IMAGELABEL_H
