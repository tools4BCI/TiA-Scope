#include "channel_graphics_object.h"
#include "base/helpers.h"

#include <QPainter>
#include <QGraphicsSceneWheelEvent>
#include <QMenu>

#include <iostream>

namespace tobiss { namespace scope {

namespace ChannelGraphicsObjectHelper
{
    void drawZeroLine (QPainter* painter, int start, int end);
}

//-----------------------------------------------------------------------------
ChannelGraphicsObject::ChannelGraphicsObject (SignalTypeFlag signal, int channel,
                                              int sampling_rate, QSharedPointer<DataBuffer const> data_buffer,
                                              QSharedPointer<SignalViewSettings> view_settings,
                                              QGraphicsItem *parent) :
    BaseGraphicsObject (parent),
    signal_ (signal),
    channel_ (channel),
    sampling_rate_ (sampling_rate),
    data_buffer_ (data_buffer),
    height_ (0),
    view_settings_ (view_settings)
{
    cyclic_start_ = QPointF (0, 0);
    error_ = 0;
    connect (view_settings_.data(), SIGNAL(channelOverlappingChanged()), SLOT(updateOverlapping()));
}

//-----------------------------------------------------------------------------
QRectF ChannelGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width (), height_);
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::setYPos (int y_pos)
{
    setY (y_pos);
    Q_EMIT bottomYChanged (y_pos + height_ );
    Q_EMIT overlappingBottomYChanged (y_pos + ((1.0 - view_settings_->getChannelOverlapping()) * height_ ));
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::setHeight (int height)
{
    height_ = height;
    Q_EMIT bottomYChanged (y() + height_);
    Q_EMIT overlappingBottomYChanged (y() + ((1.0 - view_settings_->getChannelOverlapping()) * height_ ));
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::updateView ()
{
    double seconds_to_display = view_settings_->getSignalVisualisationTime();
    data_.resize (seconds_to_display * sampling_rate_);
    qreal x_step = width ();
    x_step /= sampling_rate_ * seconds_to_display;
    data_buffer_->lockForRead();
    int new_samples = data_buffer_->numberNewSamples (signal_, channel_);
    cyclic_start_.setX (cyclic_start_.x() + x_step * new_samples);
    if (cyclic_start_.x() >= width ())
        cyclic_start_.setX (0);

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
    Q_EMIT bottomYChanged (y() + height_);
    Q_EMIT overlappingBottomYChanged (y() + ((1.0 - view_settings_->getChannelOverlapping()) * height_ ));
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
    painter->setClipping (true);
    painter->setClipRect (boundingRect());
    if (view_settings_->getChannelOverlapping() == 0)
        painter->drawRect (boundingRect());
    painter->translate (0, height_ / 2);
    bool cyclic_mode = view_settings_->getCyclicMode();

    QPointF first (width (), 0);
    QPointF second (width (), 0);

    double seconds_to_display = view_settings_->getSignalVisualisationTime();

    qreal x_step = width ();
    x_step /= sampling_rate_ * seconds_to_display;

    painter->setPen (Qt::red);
    if (cyclic_mode)
        painter->drawLine (cyclic_start_.x(), -height_ / 2, cyclic_start_.x(), height_ / 2);
    painter->setPen (Qt::darkBlue);
    if (cyclic_mode)
        first = cyclic_start_;

    ChannelGraphicsObjectHelper::drawZeroLine (painter, 0, width ());

    bool first_run = true;

    for (int sample = data_.size() - 1; sample >= 0; sample--)
    {
        qreal y = 0;
        y = data_[sample] * yScalingFactor() * view_settings_->getBasicYScaling();
        first.setX (first.x() - x_step);
        if (first.x() <= 0 && cyclic_mode)
        {
            first.setX (width ());
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

} } // namespace
