#ifndef CHANNEL_GRAPHICS_OBJECT_H
#define CHANNEL_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"
#include "base/signal_view_settings.h"
#include "base_graphics_object.h"
#include "data_collector/filters.h"

#include <QGraphicsObject>
#include <QTime>
#include <QVarLengthArray>

namespace TiAScope {

//-------------------------------------------------------------------------------------------------
class ChannelGraphicsObject : public BaseGraphicsObject
{
    Q_OBJECT
public:
    explicit ChannelGraphicsObject (SignalTypeFlag signal, int channel, QString channel_str,
                                    int sampling_rate, QSharedPointer<DataBuffer const> data_buffer,
                                    QSharedPointer<SignalViewSettings> view_settings,
                                    int width, int height, ChannelGraphicsObject *previous_channel = 0,
                                    QGraphicsItem *parent = 0);

    //virtual ~ChannelGraphicsObject () {if (ds_filter_) delete ds_filter_;}

    virtual QRectF boundingRect() const;

    int getLabelWidth() const;
    void setLabelWidth(int width);
    void setLabelVisible(bool visible);

Q_SIGNALS:
    void overlappingBottomYChanged (int overlapped_bottom_y);

public Q_SLOTS:
    void updateView ();
    void updateData ();
    void updateOverlapping ();

private Q_SLOTS:
    void hide ();
    void emitOverlappingBottomChanges (int bottom_y);

private:
    int bottomY ();
    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);    

    ChannelGraphicsObject *previous_channel_;
    SignalTypeFlag const signal_;
    int channel_;
    int sampling_rate_;
    QSharedPointer<DataBuffer const> data_buffer_;
    QSharedPointer<SignalViewSettings> view_settings_;

    QGraphicsSimpleTextItem* label_item_;

    QPointF cyclic_start_;

    QVarLengthArray<double> data_;
    int number_new_samples_;
    QTime time_;
    int error_;
    int label_width_;

    int ds_factor_, ds_pos_;
    qreal spatial_resolution_;
    qreal last_overflow_pos_;
};

} // TiAScope

#endif // CHANNEL_GRAPHICS_OBJECT_H
