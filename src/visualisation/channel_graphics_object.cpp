#include "channel_graphics_object.h"
#include "base/helpers.h"

#include <QPainter>
#include <QGraphicsSceneWheelEvent>
#include <QMenu>
#include <QDebug>

#include <iostream>

namespace TiAScope {

namespace ChannelGraphicsObjectHelper
{
    void drawZeroLine (QPainter* painter, int start, int end);
}

//-----------------------------------------------------------------------------
ChannelGraphicsObject::ChannelGraphicsObject (SignalTypeFlag signal, int channel, QString channel_str,
                                              int sampling_rate, QSharedPointer<DataBuffer const> data_buffer,
                                              QSharedPointer<SignalViewSettings> view_settings,
                                              int width, int height, ChannelGraphicsObject *previous_channel,
                                              QGraphicsItem *parent) :
    BaseGraphicsObject (parent),
    previous_channel_ (previous_channel),
    signal_ (signal),
    channel_ (channel),
    sampling_rate_ (sampling_rate),
    data_buffer_ (data_buffer),
    view_settings_ (view_settings),
    number_new_samples_ (0)
{
    cyclic_start_ = QPointF (0, 0);
    error_ = 0;
    connect (this, SIGNAL(bottomYChanged(int)), SLOT(emitOverlappingBottomChanges(int)));
    connect (view_settings_.data(), SIGNAL(channelOverlappingChanged()), SLOT(updateOverlapping()));

    setWidth(width);
    setHeight(height);

    label_item_ = new QGraphicsSimpleTextItem (channel_str, this);    
    label_width_ = label_item_->boundingRect().width();

    label_item_->setY(height/2 - label_item_->boundingRect().height()/2);

}

//-----------------------------------------------------------------------------
QRectF ChannelGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width (), height ());
}

//-----------------------------------------------------------------------------
int ChannelGraphicsObject::getLabelWidth() const
{
    return label_item_->boundingRect().width();
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::setLabelWidth(int width)
{    
    label_width_ = width;
}
//-----------------------------------------------------------------------------
void ChannelGraphicsObject::setLabelVisible(bool visible)
{
    label_item_->setVisible(visible);
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::updateView ()
{
    double seconds_to_display = view_settings_->getSignalVisualisationTime();
    data_.resize (seconds_to_display * sampling_rate_);
    qreal x_step = width () - label_width_;
    x_step /= sampling_rate_ * seconds_to_display;
    data_buffer_->lockForRead();
    number_new_samples_ = data_buffer_->numberNewSamples (signal_, channel_);
    cyclic_start_.setX (cyclic_start_.x() + (x_step * number_new_samples_));
    if (cyclic_start_.x() > (width () - label_width_))
        cyclic_start_.setX (cyclic_start_.x() - width () + label_width_);


    data_buffer_->getData (signal_, channel_, data_);
    data_buffer_->unlockForRead();


    update ();
}

//-------------------------------------------------------------------------------------------------
void ChannelGraphicsObject::hide ()
{
    view_settings_->setChannelVisibility (signal_, channel_, false);
}

//-------------------------------------------------------------------------------------------------
void ChannelGraphicsObject::updateOverlapping ()
{
    Q_EMIT bottomYChanged (bottomY ());
}

//-------------------------------------------------------------------------------------------------
void ChannelGraphicsObject::emitOverlappingBottomChanges (int)
{
    Q_EMIT overlappingBottomYChanged (y() + ((1.0 - view_settings_->getChannelOverlapping()) * height ()));
}

//-------------------------------------------------------------------------------------------------
int ChannelGraphicsObject::bottomY ()
{
    if (previous_channel_)
        return std::max<int> (height () + y(), previous_channel_->bottomY());
    else
        return y() + height ();
}

//-------------------------------------------------------------------------------------------------
void ChannelGraphicsObject::contextMenuEvent (QGraphicsSceneContextMenuEvent *event)
{
    QMenu context_menu;;
    QAction* hide_action = context_menu.addAction ("Hide");
    connect (hide_action, SIGNAL(triggered()), SLOT(hide()));
    context_menu.exec (event->screenPos());
}

//-------------------------------------------------------------------------------------------------
void ChannelGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen (Qt::black);
    painter->setClipping (false);
    painter->setClipRect (boundingRect());
    //if (view_settings_->getChannelOverlapping() == 0)
    //    painter->drawRect (boundingRect());

    double y_scaling = yScalingFactor() * view_settings_->getBasicYScaling();
    painter->translate (label_width_, height() / 2);
    if (view_settings_->autoScalingEnabled())
    {
        double max = qAbs (data_buffer_->getMax (signal_, channel_));
        double min = qAbs (data_buffer_->getMin (signal_, channel_));
        double scale = (height() - 2)  / 2;
        scale /= qMax (max, min);
        y_scaling = scale * view_settings_->getBasicYScaling();
    }

    bool cyclic_mode = view_settings_->getCyclicMode();

    QPointF first (width () - label_width_, 0);
    QPointF second (width () - label_width_, 0);

    double seconds_to_display = view_settings_->getSignalVisualisationTime();

    qreal x_step = width () - label_width_;
    x_step /= sampling_rate_ * seconds_to_display;


    painter->setPen (Qt::red);
    if (cyclic_mode)
        painter->drawLine (cyclic_start_.x(), -height() / 2, cyclic_start_.x(), height() / 2);
    painter->setPen (Qt::darkBlue);
    if (cyclic_mode)
        first = cyclic_start_;

    ChannelGraphicsObjectHelper::drawZeroLine (painter, 0, width () - label_width_);

    bool first_run = true;

    qDebug() << "Buffer size: " << data_.size() << "new samples: " << number_new_samples_ << "x_step: " << x_step;

    for (int sample = data_.size() - 1; sample >= 0; sample--)
    {

        qreal y = 0;
        y = data_[sample] * y_scaling;
        first.setX (first.x() - x_step);
        if (first.x() <= 0 && cyclic_mode)
        {
            first.setX (width () - label_width_);
            first_run = true;
        }
        first.setY (y);
        if (!first_run)
            painter->drawLine (first, second);
        else
            first_run = false;
        second = first;


    }
}

//-------------------------------------------------------------------------------------------------
namespace ChannelGraphicsObjectHelper
{
    //---------------------------------------------------------------------------------------------
    void drawZeroLine (QPainter* painter, int start, int end)
    {
        QColor old_color = painter->pen ().color();
        painter->setPen (Qt::lightGray);
        painter->drawLine (start, 0, end, 0);
        painter->setPen (old_color);
    }
}

} // TiAScope
