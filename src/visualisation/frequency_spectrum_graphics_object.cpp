#include "frequency_spectrum_graphics_object.h"

#include <QPainter>
#include <QDebug>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
FrequencySpectrumGraphicsObject::FrequencySpectrumGraphicsObject (QString const& signal, int channel, QGraphicsItem *parent) :
    BaseGraphicsObject (parent),
    SIGNAL_ (signal),
    CHANNEL_ (channel),
    height_ (0)
{

}

//-------------------------------------------------------------------------------------------------
QRectF FrequencySpectrumGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width_, height_);
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::updateData (QVector<double> data, QString const& signal, int channel, int frequency_range)
{
    if ((channel != CHANNEL_) || (signal != SIGNAL_))
        return;
    data_.resize (data.size());
    for (int i = 0; i < data.size(); i++)
        data_[i] = data[i];
    frequency_range_ = frequency_range;
    update ();
}


//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (data_.size() == 0)
        return;
    painter->translate (0, height_ / 2);

    painter->setPen(Qt::blue);
    painter->drawLine (0, 0, width_, 0);
    painter->setPen (Qt::blue);

    qreal x_step = width_;
    x_step /= data_.size();

    qreal x_pos = 0;

    for (int index = 0; index < data_.size(); index++)
    {
        painter->drawLine (QPointF(x_pos, 0), QPointF(x_pos, -data_[index] * yScalingFactor()));
        x_pos += x_step;
    }

    painter->setPen(Qt::black);
    painter->drawLine (0, 1, 0, 5);
    painter->drawLine (width_ - 1, 1, width_ - 1, 5);
    painter->drawText (0, 5, LABEL_SPACING_, 20, Qt::AlignVCenter | Qt::AlignLeft,  "0.0Hz");
    painter->drawText (width_ - LABEL_SPACING_ - 1, 5, LABEL_SPACING_, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number (frequency_range_, 'f', 1).append("Hz"));
    drawXLabelInTheMiddle (painter, 0, width_);
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::drawXLabelInTheMiddle (QPainter* painter, qreal left, qreal right)
{
    qreal middle_pos = (right + left) / 2;

    painter->drawLine (middle_pos, 1, middle_pos, 5);
    painter->drawText (middle_pos - (LABEL_SPACING_ / 2), 5, LABEL_SPACING_, 20, Qt::AlignCenter, QString::number(static_cast<double>(frequency_range_ * middle_pos) / width_, 'f', 1));

    if ((right - left) > LABEL_SPACING_)
    {
        drawXLabelInTheMiddle (painter, left, middle_pos);
        drawXLabelInTheMiddle (painter, middle_pos, right);
    }
}

} } // namespace
