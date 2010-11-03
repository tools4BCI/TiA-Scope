#include "signal_graphics_object.h"
#include "channel_graphics_object.h"
#include "frequency_spectrum_graphics_object.h"
#include "aperiodic_data_graphics_object.h"
#include "base/helpers.h"


#include "definitions/constants.h"

#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneWheelEvent>
#include <QDebug>

namespace tobiss { namespace scope {

//-----------------------------------------------------------------------------
SignalGraphicsObject::SignalGraphicsObject (Signal const& signal, QSharedPointer<DataBuffer const> data_buffer, QSharedPointer<SignalViewSettings> view_settings,
                                            FourierTransformThread* ft_thread, QGraphicsItem *parent) :
    QGraphicsObject (parent),
    height_ (0),
    width_ (900),
    signal_type_ (TypeConverter::stdStringToSignalTypeFlag (signal.type())),
    aperiodic_signal_ (0),
    view_settings_ (view_settings)
{
    QGraphicsSimpleTextItem* signal_label = new QGraphicsSimpleTextItem (signal.type().c_str(), this);

    int channel_amount = signal.channels().size();
    if (signal_type_ & (SIG_BUTTON | SIG_JOYSTICK))
    {
        height_ = 200;
        aperiodic_signal_ = AperiodicDataGraphicsObject::createAperiodicDataGraphicsObject (signal_type_, data_buffer, this);
    }
    else
    {
        if (ft_thread)
            connect (ft_thread, SIGNAL (FTEnabledChanged (SignalTypeFlag, int, bool)), SLOT(ftEnabled(SignalTypeFlag,int,bool)));
    for (int channel_index = 0; channel_index < channel_amount; channel_index++)
    {
        height_ += 200;
        ChannelGraphicsObject* channel = new ChannelGraphicsObject (signal_type_, channel_index, signal.samplingRate(), data_buffer, view_settings, this);
        channel->setPos (0, 200 * channels_.size());
        channel->setHeight (200);
        channel->setWidth (width_ - 50);
        channels_[channel_index] = channel;
        children_.append (channel);
        QGraphicsSimpleTextItem* channel_label = new QGraphicsSimpleTextItem (signal.channels()[channel_index].id().c_str (), channel);
        channel_label->setPos (400, channel->height() / 2);
        if (ft_thread)
        {
            FrequencySpectrumGraphicsObject* ft_channel = new FrequencySpectrumGraphicsObject (signal_type_, channel_index, this);
            ft_channel->setPos (450, 200 * (channels_.size() - 1));
            ft_channel->setHeight (200);
            ft_channel->setWidth (400);
            ft_channel->connect (ft_thread, SIGNAL(FTFinished(QVector<double>, SignalTypeFlag, int, int)), SLOT(updateData (QVector<double>, SignalTypeFlag, int, int)), Qt::BlockingQueuedConnection);
            fts_[channel_index] = ft_channel;
            children_.append (ft_channel);
        }
    }
    }
}

//-----------------------------------------------------------------------------
QRectF SignalGraphicsObject::boundingRect() const
{
    return QRectF (0, 0, width_, height_);
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
    ChannelGraphicsObject* prev_channel = 0;
    Q_FOREACH (ChannelGraphicsObject* channel, channels_.values())
    {
        channel->setHeight (height_ / channels_.size());
        if (prev_channel)
            channel->setPos (0, prev_channel->pos().y() + prev_channel->height());
        prev_channel = channel;
    }
}


//-------------------------------------------------------------------------------------------------
void SignalGraphicsObject::ftEnabled (SignalTypeFlag signal, int channel, bool enbaled)
{
    if (signal == signal_type_)
    {
        if (enbaled)
            helpers::animateProperty (channels_[channel], "width", channels_[channel]->width(), (width_ / 2) - 50);
        else
            helpers::animateProperty (channels_[channel], "width", channels_[channel]->width(), width_ - 50);
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
            view_settings_->setBasicYScaling (view_settings_->getBasicYScaling() * 2);
        else if (event->delta() < 0)
            view_settings_->setBasicYScaling (view_settings_->getBasicYScaling() / 2);
    }
    else
        event->ignore();
}


} } // namespace
