#include "signal_graphics_object.h"
#include "channel_graphics_object.h"
#include "frequency_spectrum_graphics_object.h"
#include "aperiodic_data_graphics_object.h"
#include "base/helpers.h"

#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>

namespace TiAScope {

//-----------------------------------------------------------------------------
SignalGraphicsObject::SignalGraphicsObject (TiAQtImplementation::SignalTypeFlag signal_type,
                                            TiAQtImplementation::TiAMetaInfo const& signal_info,
                                            QSharedPointer<DataBuffer const> data_buffer, QSharedPointer<SignalViewSettings> view_settings,
                                            QSharedPointer<FTViewSettings> ft_view_settings, FourierTransformThread* ft_thread, QGraphicsItem *parent) :
    QGraphicsObject (parent),
    width_ (100),
    height_ (100),
    signal_type_ (signal_type),
    aperiodic_signal_ (0),
    view_settings_ (view_settings),
    ft_view_settings_ (ft_view_settings)
{
    new QGraphicsSimpleTextItem (TiAQtImplementation::signalTypeFlagToString (signal_type), this);

    int channel_amount = signal_info.getNumChannels (signal_type_);
    if (TiAQtImplementation::isAperiodic (signal_type_))
    {
        // TODO: deactivated aperiodic signals
        // aperiodic_signal_ = AperiodicDataGraphicsObject::createAperiodicDataGraphicsObject (signal_type_, data_buffer, this);
    }
    else
    {
        connect (view_settings_.data(), SIGNAL(channelVisibilityChanged (SignalTypeFlag, ChannelID, bool)), SLOT(setChannelVisibility(SignalTypeFlag,ChannelID,bool)));
        if (ft_thread)
        {
            FrequencySpectrumGraphicsObject* previous_channel = 0;
            for (int channel_index = 0; channel_index < channel_amount; channel_index++)
            {
                FrequencySpectrumGraphicsObject* ft_channel = new FrequencySpectrumGraphicsObject (signal_type_, channel_index, ft_view_settings_, this);
                ft_channel->connect (ft_thread, SIGNAL(FTEnabledChanged(SignalTypeFlag,int,bool)), SLOT(enableDrawing(SignalTypeFlag,int,bool)));
                if (previous_channel)
                {
                    ft_channel->connect (previous_channel, SIGNAL(bottomYChanged(int)), SLOT(setYPos(int)));
                    ft_channel->setPos (0, 0);
                }
                ft_channel->setHeight (0);
                ft_channel->setWidth (width_);
                ft_channel->connect (ft_thread, SIGNAL(FTFinished(QVector<double>, SignalTypeFlag, int, int)), SLOT(updateData (QVector<double>, SignalTypeFlag, int, int)), Qt::BlockingQueuedConnection);
                fts_[channel_index] = ft_channel;
                children_.append (ft_channel);
                previous_channel = ft_channel;
            }
            connect (previous_channel, SIGNAL(bottomYChanged(int)), SLOT(setHeight(int)));
            fts_[0]->setYPos (0);
        }
        else
        {
            ChannelGraphicsObject* previous_channel = 0;
            for (int channel_index = 0; channel_index < channel_amount; channel_index++)
            {
                ChannelGraphicsObject* channel = new ChannelGraphicsObject (signal_type_, channel_index, signal_info.getSamplingRate(signal_type_), data_buffer, view_settings, previous_channel, this);
                if (previous_channel)
                    channel->connect (previous_channel, SIGNAL(overlappingBottomYChanged(int)), SLOT(setYPos(int)));

                channel->setPos (0, ChannelGraphicsObject::defaultHeight() * channels_.size());
                channel->setHeight (ChannelGraphicsObject::defaultHeight());
                channel->setWidth (width_);
                channels_[channel_index] = channel;
                children_.append (channel);
                QGraphicsSimpleTextItem* channel_label = new QGraphicsSimpleTextItem (signal_info.getChannelLabel(signal_type_, channel_index), channel);
                channel_label->setPos (400, channel->height() / 2);
                previous_channel = channel;
            }
            connect (previous_channel, SIGNAL(bottomYChanged(int)), SLOT(setHeight(int)));
            channels_[0]->updateOverlapping();
        }
    }
}

//-----------------------------------------------------------------------------
QRectF SignalGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width_, height_);
}

//-----------------------------------------------------------------------------
int SignalGraphicsObject::height () const
{
    return height_;
}

//-----------------------------------------------------------------------------
int SignalGraphicsObject::width () const
{
    return width_;
}

//-----------------------------------------------------------------------------
void SignalGraphicsObject::updateToDataBuffer ()
{
    Q_FOREACH (ChannelGraphicsObject* channel, channels_.values())
        channel->updateView ();
    if (aperiodic_signal_)
        aperiodic_signal_->updateView ();
    update ();
}

//-----------------------------------------------------------------------------
void SignalGraphicsObject::setHeight (int height)
{
    height_ = height;
    Q_EMIT bottomYChanged (y() + height);
}

//-----------------------------------------------------------------------------
void SignalGraphicsObject::setWidth (int width)
{
    width_ = width;
    Q_FOREACH (BaseGraphicsObject* child, children_)
    {
        child->setWidth (width_);
    }
}

//-------------------------------------------------------------------------------------------------
void SignalGraphicsObject::setChannelVisibility (SignalTypeFlag signal, ChannelID channel, bool visible)
{
    if (signal == signal_type_ && fts_.size() == 0)
    {
        if (visible)
            helpers::animateProperty (channels_[channel], "height", channels_[channel]->height(), ChannelGraphicsObject::defaultHeight());
        else
            helpers::animateProperty (channels_[channel], "height", channels_[channel]->height(), 0);
    }
}


//-------------------------------------------------------------------------------------------------
void SignalGraphicsObject::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen = painter->pen();
    pen.setWidth (2);
    painter->setPen (pen);
    painter->drawRect (boundingRect());
}

//-----------------------------------------------------------------------------
void SignalGraphicsObject::wheelEvent (QGraphicsSceneWheelEvent* event)
{
    if (event->modifiers().testFlag (Qt::ShiftModifier))
    {
        event->accept ();
        if (event->delta() > 0)
        {
            Q_FOREACH (BaseGraphicsObject* obj, children_)
                obj->setYScalingFactor (obj->yScalingFactor() * 2);
        }
        else if (event->delta() < 0)
        {
            Q_FOREACH (BaseGraphicsObject* obj, children_)
                obj->setYScalingFactor (obj->yScalingFactor() / 2);
        }
    }
    else
        event->ignore();
}


} // TiAScope
