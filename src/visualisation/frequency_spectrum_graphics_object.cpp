#include "frequency_spectrum_graphics_object.h"

#include "base/helpers.h"

#include <QPainter>
#include <QDebug>
#include <QSettings>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
FrequencySpectrumGraphicsObject::FrequencySpectrumGraphicsObject (SignalTypeFlag signal, int channel, QGraphicsItem *parent) :
    BaseGraphicsObject (parent),
    SIGNAL_ (signal),
    CHANNEL_ (channel),
    buffer_ (100, 100),
    buffer_current_pos_ (0),
    enabled_ (false)
{
    buffer_ = QPixmap (100, 200);
}

//-------------------------------------------------------------------------------------------------
QRectF FrequencySpectrumGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width (), height ());
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::updateData (QVector<double> data, SignalTypeFlag signal, int channel, int frequency_range)
{
    if ((channel != CHANNEL_) || (signal != SIGNAL_) || !enabled_)
        return;

    frequency_range_ = frequency_range;

    // FIXME: put following into a method!!
    // updating pixmap buffer here
    QPainter painter (&buffer_);
    QColor color;

    int x_pos = buffer_current_pos_;
    int y_pos = 0;
    int y_step = 1;
    int x_step = 1;

    int hsv_value = 240;


    QSettings settings;
    lower_index_ = settings.value ("fourier/lower_bound", 0).toInt();
    upper_index_ = settings.value ("fourier/upper_bound", 100).toInt();
    lower_index_ *= data.size ();
    upper_index_ *= data.size ();
    lower_index_ /= frequency_range_;
    upper_index_ /= frequency_range_;
    max_index_ = data.size ();

    for (int index = upper_index_; index > lower_index_; index--)
    {
        hsv_value = 240 - (data[index] * yScalingFactor() *  240);
        if (hsv_value < 0)
            hsv_value = 0;
        color.setHsv (hsv_value, 255, 255);
        painter.fillRect (QRectF (QPointF(x_pos, y_pos), QPointF (x_pos + x_step, y_pos + y_step)), color);
        y_pos += y_step;
    }

    buffer_current_pos_++;
    if (buffer_current_pos_ > buffer_.width())
        buffer_current_pos_ = 0;

    update ();
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::enableDrawing (SignalTypeFlag signal, int channel, bool enabled)
{
    bool newly_enabled = false;
    bool newly_disabled = false;
    if (SIGNAL_ == signal && CHANNEL_ == channel)
    {
        newly_enabled = (!enabled_) && (enabled);
        newly_disabled = (enabled_) && (!enabled);
        enabled_ = enabled;
    }
    if (newly_enabled)
        helpers::animateProperty (this, "height", 0, defaultHeight());
    if (newly_disabled)
        helpers::animateProperty (this, "height", height(), 0);
}

//-------------------------------------------------------------------------------------------------
void FrequencySpectrumGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (!enabled_)
        return;

    //painter->setClipping (true);
    //painter->setClipRect (boundingRect());
//    if (history_.size() == 0)
//        return;

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
    int y_axis_width = 100;
    int drawing_width = width() - y_axis_width;
    qreal adapted_pos = buffer_current_pos_ * drawing_width;
    adapted_pos /= buffer_.width();
//    painter->drawPixmap (QRectF (0, 0, drawing_width, height()), buffer_, QRectF (0, 0, buffer_.width(), upper_index_ - lower_index_));
    painter->drawPixmap (QRectF (drawing_width - adapted_pos, 0, adapted_pos, height()), buffer_, QRectF (0, 0, buffer_current_pos_, upper_index_ - lower_index_));
    painter->drawPixmap (QRectF (0, 0, drawing_width - adapted_pos, height()), buffer_, QRectF (buffer_current_pos_, 0, buffer_.width() - buffer_current_pos_, upper_index_ - lower_index_));

    painter->drawText (drawing_width + 5, height() - 5, QString::number (lower_index_ * frequency_range_ / max_index_).append("Hz"));
    painter->drawText (drawing_width + 5, height() / 2, QString::number (((lower_index_ + upper_index_) / 2) * frequency_range_ / max_index_).append("Hz"));
    painter->drawLine (drawing_width, height() / 2, drawing_width + 5, height() / 2);
    painter->drawText (drawing_width + 5, 12, QString::number (upper_index_ * frequency_range_ / max_index_).append("Hz"));
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
