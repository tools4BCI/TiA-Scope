#ifndef FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H
#define FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H

#include "base_graphics_object.h"

#include <QVector>
#include <QSharedPointer>

namespace tobiss { namespace scope {

class FrequencySpectrumGraphicsObject : public BaseGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (int height READ height WRITE setHeight)
    Q_PROPERTY (int width READ width WRITE setWidth)
public:
    explicit FrequencySpectrumGraphicsObject (QString const& signal, int channel, QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
    int height () const {return height_;}
    int width () const {return width_;}

public Q_SLOTS:
    void setHeight (int height) {height_ = height;}
    void setWidth (int width) {width_ = width;}

    void updateData (QVector<double> data, QString const& signal, int channel, int frequency_range);

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    inline void drawXLabelInTheMiddle (QPainter *painter, qreal left, qreal right);

    QString const SIGNAL_;
    int const CHANNEL_;
    int height_;
    int width_;

    static qreal const LABEL_SPACING_ = 200;

    int frequency_range_;
    QVector<double> data_;
};

} } // namespace

#endif // FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H
