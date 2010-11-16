#ifndef CHANNEL_GRAPHICS_OBJECT_H
#define CHANNEL_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"
#include "base/signal_view_settings.h"
#include "base_graphics_object.h"

#include <QGraphicsObject>
#include <QTime>
#include <QVarLengthArray>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
class ChannelGraphicsObject : public BaseGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (int height READ height WRITE setHeight)
public:
    explicit ChannelGraphicsObject (SignalTypeFlag signal, int channel,
                                    int sampling_rate, QSharedPointer<DataBuffer const> data_buffer,
                                    QSharedPointer<SignalViewSettings> view_settings,
                                    ChannelGraphicsObject *previous_channel = 0,
                                    QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;

    int height () const {return height_;}

    static int defaultHeight () {return 200;}

Q_SIGNALS:
    void bottomYChanged (int bottom_y);
    void overlappingBottomYChanged (int overlapped_bottom_y);

public Q_SLOTS:
    void setYPos (int y_pos);
    void setHeight (int height);
    void updateView ();

private Q_SLOTS:
    void hide ();
    void updateOverlapping ();

private:
    void emitBottomChanges ();
    int bottomY ();
    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    ChannelGraphicsObject *previous_channel_;
    SignalTypeFlag const signal_;
    int channel_;
    int sampling_rate_;
    QSharedPointer<DataBuffer const> data_buffer_;
    int height_;
    QSharedPointer<SignalViewSettings> view_settings_;

    QPointF cyclic_start_;

    QVarLengthArray<double> data_;
    QTime time_;
    int error_;
};

} } // namespace

#endif // CHANNEL_GRAPHICS_OBJECT_H
