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
    number_new_samples_ (0),    
    ds_factor_ (1),
    spatial_resolution_ (-1.0)
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
    update();
}

//-----------------------------------------------------------------------------
void ChannelGraphicsObject::updateData ()
{
    double seconds_to_display = view_settings_->getSignalVisualisationTime();
    data_.resize (seconds_to_display * sampling_rate_);
    qreal x_step = width () - label_width_;
    x_step /= sampling_rate_ * seconds_to_display;
    data_buffer_->lockForRead();
    number_new_samples_ = data_buffer_->numberNewSamples (signal_, channel_);
    data_buffer_->getData (signal_, channel_, data_);
    data_buffer_->unlockForRead();    

    // downsample datastream if the x-step size is too small

    // resolution has changed => compute new ds filter
    if (qAbs<qreal>(spatial_resolution_ - x_step) > 0.0001){

        if(x_step < 0.75) {
            ds_factor_ = (int)( 0.75/x_step);
        }
        else {
            ds_factor_ = 1;
        }

        spatial_resolution_ = x_step;
        ds_pos_ = 0;
    }

    ds_pos_ = (ds_pos_ + number_new_samples_) % ds_factor_;

    cyclic_start_.setX (cyclic_start_.x() + x_step * number_new_samples_);

    if (cyclic_start_.x() > (width () - label_width_)){

        cyclic_start_.setX (cyclic_start_.x() - width () + label_width_);

    }

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
    QMenu context_menu;
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
    painter->translate (label_width_, height()/2);
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

    painter->setPen (Qt::darkBlue);
    if (cyclic_mode)
        first = cyclic_start_;

    bool first_run = true;        

    qreal min_pix_y_value = 0.0;
    qreal max_pix_y_value = 0.0;    

    qDebug() << "Buffer size: " << data_.size() << "new samples: " << number_new_samples_;
    qDebug() << "ds factor: " << ds_factor_ << "spatial_res: " << spatial_resolution_;   


    ChannelGraphicsObjectHelper::drawZeroLine (painter, 0, width () - label_width_);

    int start_sample = data_.size() - 1;

    if (ds_factor_ > 1){

        start_sample = start_sample - ds_pos_;
        first.setX(first.x() - spatial_resolution_*ds_pos_);
    }

    for (int sample = start_sample; sample >= 0; sample = sample - ds_factor_)
    {

        if (ds_factor_ > 1){
            max_pix_y_value = min_pix_y_value = data_[sample] * y_scaling;

            for (int subsample = sample; subsample > sample - ds_factor_ && subsample > 0; subsample--) {
                max_pix_y_value = max_pix_y_value < data_[subsample] * y_scaling ? data_[subsample] * y_scaling : max_pix_y_value;
                min_pix_y_value = min_pix_y_value > data_[subsample] * y_scaling ? data_[subsample] * y_scaling : min_pix_y_value;                
            }
            first.setX(first.x() - spatial_resolution_*ds_factor_);
            first.setY(max_pix_y_value);

        }else{

            qreal y = 0;
            y = data_[sample] * y_scaling;
            first.setX (first.x() - spatial_resolution_);
            first.setY (y);
        }

        if (first.x() <= 0 && cyclic_mode)
        {
            first.setX (first.x() + width () - label_width_);
            first_run = true;
        }

        if (!first_run){

            if (ds_factor_ > 1){
                painter->drawLine (first, second);

                second = first;
                first.setY(min_pix_y_value);
            }                        

            painter->drawLine (first, second);
        }
        else
            first_run = false;
        second = first;
    }

    if (cyclic_mode) {
        painter->setPen (Qt::red);
        painter->drawLine (cyclic_start_.x(), -height()/2, cyclic_start_.x(), height()/2);
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
