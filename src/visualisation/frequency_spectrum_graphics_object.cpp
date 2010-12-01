#include "frequency_spectrum_graphics_object.h"

#include <QPainter>
#include <QDebug>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
FrequencySpectrumGraphicsObject::FrequencySpectrumGraphicsObject (SignalTypeFlag signal, int channel, QGraphicsItem *parent) :
    BaseGraphicsObject (parent),
    SIGNAL_ (signal),
    CHANNEL_ (channel)
{

}

//-------------------------------------------------------------------------------------------------
QRectF FrequencySpectrumGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width (), height ());
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::updateData (QVector<double> data, SignalTypeFlag signal, int channel, int frequency_range)
{
    if ((channel != CHANNEL_) || (signal != SIGNAL_))
        return;
    history_.push_back (data);
    if (history_.size() > 30)
        history_.pop_front ();
    //data_.resize (data.size());
    //for (int i = 0; i < data.size(); i++)
    //    data_[i] = data[i]; // add log10 or ln here??
    frequency_range_ = frequency_range;
    update ();
}


//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setClipping (true);
    painter->setClipRect (boundingRect());
    if (history_.size() == 0)
        return;

    colouredVisualisation (painter);
    return;

//    painter->translate (0, height () * 3 / 4);

//    painter->setPen(Qt::blue);
//    painter->drawLine (0, 0, width (), 0);
//    painter->setPen (Qt::blue);

//    qreal x_step = width ();
//    x_step /= data_.size();

//    qreal x_pos = 0;

//    for (int index = 0; index < data_.size(); index++)
//    {
//        if (data_[index] > 0)
//            painter->drawRect (QRectF (QPointF(x_pos, -data_[index] * yScalingFactor()), QPointF (x_pos + x_step, 0)));
//        x_pos += x_step;
//    }

//    painter->setPen(Qt::black);
//    painter->drawLine (0, 1, 0, 5);
//    painter->drawLine (width () - 1, 1, width () - 1, 5);
//    painter->drawText (0, 5, LABEL_SPACING_, 20, Qt::AlignVCenter | Qt::AlignLeft,  "0.0Hz");
//    painter->drawText (width () - LABEL_SPACING_ - 1, 5, LABEL_SPACING_, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number (frequency_range_, 'f', 1).append("Hz"));
//    drawXLabelInTheMiddle (painter, 0, width ());
}


//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::colouredVisualisation (QPainter *painter)
{
    if (history_.size() == 0)
        return;

    qreal x_step = width ();
    x_step /= history_.size();

    qreal x_pos = 0;
    qreal y_pos = 0;

    QColor color;
    int hsv_value = 240;
//    color.setHsv (hsv_value, 255, 255);

    qreal y_step = 0;
    Q_FOREACH (QVector<double> data, history_)
    {
        y_pos = 0;
        y_step = height ();
        y_step /= data.size ();
        for (int index = 0; index < data.size(); index++)
        {
            hsv_value = 240 - (data[index] * yScalingFactor() *  240);
            if (hsv_value < 0)
                hsv_value = 0;
            color.setHsv (hsv_value, 255, 255);
            painter->fillRect (QRectF (QPointF(x_pos, y_pos), QPointF (x_pos + x_step, y_pos + y_step)), color);
            y_pos += y_step;
        }
        x_pos += x_step;
    }
}


//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::drawXLabelInTheMiddle (QPainter* painter, qreal left, qreal right)
{
    qreal middle_pos = (right + left) / 2;

    painter->drawLine (middle_pos, 1, middle_pos, 5);
    painter->drawText (middle_pos - (LABEL_SPACING_ / 2), 5, LABEL_SPACING_, 20, Qt::AlignCenter, QString::number(static_cast<double>(frequency_range_ * middle_pos) / width (), 'f', 1));

    if ((right - left) > LABEL_SPACING_)
    {
        drawXLabelInTheMiddle (painter, left, middle_pos);
        drawXLabelInTheMiddle (painter, middle_pos, right);
    }
}

} } // namespace
