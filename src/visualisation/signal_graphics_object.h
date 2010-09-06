#ifndef SIGNAL_GRAPHICS_OBJECT_H
#define SIGNAL_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"
#include "base/signal_view_settings.h"

#include "config/ss_meta_info.h"

#include "data_collector/fourier_transform_thread.h"

#include <QGraphicsObject>
#include <QString>
#include <QMap>
#include <QSharedPointer>

namespace tobiss { namespace scope {

class ChannelGraphicsObject;
class FrequencySpectrumGraphicsObject;
class BaseGraphicsObject;
class AperiodicDataGraphicsObject;

//-------------------------------------------------------------------------------------------------
class SignalGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (int height READ height WRITE setHeight)
public:
    explicit SignalGraphicsObject (Signal const& signal,
                                   QSharedPointer<DataBuffer const> data_buffer,
                                   QSharedPointer<SignalViewSettings> view_settings,
                                   FourierTransformThread* ft_thread = 0,
                                   QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
    int height () const {return height_;}
    int defaultHeight () const {if (aperiodic_signal_) return 200; else return 200 * channels_.size();}

public Q_SLOTS:
    void updateToDataBuffer ();
    void setVisibleTrue () {setVisible (true);}
    void setVisibleFalse () {setVisible (false);}
    void setHeight (int height);

private Q_SLOTS:
    void ftEnabled (SignalTypeFlag signal, int channel, bool enbaled);


private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void wheelEvent (QGraphicsSceneWheelEvent* event);


    int height_;
    int width_;
    SignalTypeFlag signal_type_;
    QMap<int, ChannelGraphicsObject*> channels_;
    QMap<int, FrequencySpectrumGraphicsObject*> fts_;
    QList<BaseGraphicsObject*> children_;
    AperiodicDataGraphicsObject* aperiodic_signal_;
};

} } // namespace

#endif // SIGNAL_GRAPHICS_OBJECT_H
