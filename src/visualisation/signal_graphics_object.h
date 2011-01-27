#ifndef SIGNAL_GRAPHICS_OBJECT_H
#define SIGNAL_GRAPHICS_OBJECT_H

#include "base/data_buffer.h"
#include "base/signal_view_settings.h"
#include "base/ft_view_settings.h"

#include "data_collector/qt_tia_client/tia_metainfo.h"

#include "data_collector/fourier_transform_thread.h"

#include "channel_graphics_object.h"

#include <QGraphicsObject>
#include <QString>
#include <QMap>
#include <QSharedPointer>

namespace TiAScope {

class FrequencySpectrumGraphicsObject;
class BaseGraphicsObject;
class AperiodicDataGraphicsObject;

//-------------------------------------------------------------------------------------------------
class SignalGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY (int height READ height WRITE setHeight)
    Q_PROPERTY (int width READ width WRITE setWidth)
public:
    explicit SignalGraphicsObject (TiAQtImplementation::SignalTypeFlag signal_type,
                                   TiAQtImplementation::TiAMetaInfo const& signal_info,
                                   QSharedPointer<DataBuffer const> data_buffer,
                                   QSharedPointer<SignalViewSettings> view_settings,
                                   QSharedPointer<FTViewSettings> ft_view_settings = QSharedPointer<FTViewSettings>(0),
                                   FourierTransformThread* ft_thread = 0,
                                   QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
    int height () const;
    int width () const;
    int defaultHeight () const {if (aperiodic_signal_) return 200; else return ChannelGraphicsObject::defaultHeight () * channels_.size();}

Q_SIGNALS:
    void bottomYChanged (int bottom_y);

public Q_SLOTS:
    void setYPos (int y_pos) {setY (y_pos); Q_EMIT bottomYChanged(y_pos + height());}
    void updateToDataBuffer ();
    void setVisibleTrue () {setVisible (true);}
    void setVisibleFalse () {setVisible (false);}
    void setHeight (int height);
    void setWidth (int width);

private Q_SLOTS:
    void setChannelVisibility (SignalTypeFlag signal, ChannelID channel, bool visible);

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void wheelEvent (QGraphicsSceneWheelEvent* event);


    int width_;
    int height_;
    SignalTypeFlag signal_type_;
    QMap<int, ChannelGraphicsObject*> channels_;
    QMap<int, FrequencySpectrumGraphicsObject*> fts_;
    QList<BaseGraphicsObject*> children_;
    AperiodicDataGraphicsObject* aperiodic_signal_;
    QSharedPointer<SignalViewSettings> view_settings_;
    QSharedPointer<FTViewSettings> ft_view_settings_;
    QGraphicsSimpleTextItem* label_item_;
};

} // TiAScope

#endif // SIGNAL_GRAPHICS_OBJECT_H
