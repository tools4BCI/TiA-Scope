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
    explicit ChannelGraphicsObject (QString const& signal, int channel,
                                    int sampling_rate, QSharedPointer<DataBuffer const> data_buffer,
                                    QSharedPointer<SignalViewSettings> view_settings,
                                    QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;

    int height () const {return height_;}

public Q_SLOTS:
    void setHeight (int height) {height_ = height;}
    void updateView ();

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString const signal_;
    int channel_;
    int sampling_rate_;
    QSharedPointer<DataBuffer const> data_buffer_;
    int height_;
    QSharedPointer<SignalViewSettings const> view_settings_;

    QPointF cyclic_start_;

    QVarLengthArray<double> data_;
    QTime time_;
    int error_;
};

} } // namespace

#endif // CHANNEL_GRAPHICS_OBJECT_H
