#ifndef APERIODIC_DATA_GRAPHICS_OBJECT_H
#define APERIODIC_DATA_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"

#include <QSharedPointer>
#include <QGraphicsWidget>

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
class AperiodicDataGraphicsObject : public QGraphicsWidget
{
    Q_OBJECT
public:
    static AperiodicDataGraphicsObject* createAperiodicDataGraphicsObject (SignalTypeFlag signal, QSharedPointer<DataBuffer const> data_buffer, QGraphicsItem *parent);

    virtual QRectF boundingRect() const {return QRectF(0, 0, 200, 400);}

public Q_SLOTS:
    void updateView ();

protected:
    QMap<DeviceID, QVector<double> > data_;
    AperiodicDataGraphicsObject (QGraphicsItem *parent) : QGraphicsWidget (parent) {}

private:
    Q_DISABLE_COPY(AperiodicDataGraphicsObject)

    QSharedPointer<DataBuffer const> data_buffer_;
    SignalTypeFlag signal_type_;
};

} } // namespace


#endif // APERIODIC_DATA_GRAPHICS_OBJECT_H
