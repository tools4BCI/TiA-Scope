#ifndef FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H
#define FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H

#include "base_graphics_object.h"
#include "base/user_types.h"
#include "base/ft_view_settings.h"

#include <QVector>
#include <QLinkedList>
#include <QSharedPointer>
#include <QPixmap>
#include <QMutex>

namespace TiAScope {

class FrequencySpectrumGraphicsObject : public BaseGraphicsObject
{
    Q_OBJECT
public:
    explicit FrequencySpectrumGraphicsObject (SignalTypeFlag signal, int channel, QSharedPointer<FTViewSettings> view_settings,
                                              QGraphicsItem *parent = 0);

    virtual QRectF boundingRect() const;
public Q_SLOTS:
    void updateData (QVector<double> data, SignalTypeFlag signal, int channel, int frequency_range);
    void enableDrawing (SignalTypeFlag signal, int channel, bool enabled);
    void updateWholeHistory ();

private:
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateBuffer (bool whole_history);
    void colouredVisualisation (QPainter *painter);

    inline void drawXLabelInTheMiddle (QPainter *painter, qreal left, qreal right);

    SignalTypeFlag const SIGNAL_;
    int const CHANNEL_;

    static qreal const LABEL_SPACING_; // = 200;

    int frequency_range_;
    //QVector<double> data_;
    QLinkedList<QVector<double> > history_;
    QPixmap buffer_;
    int buffer_current_pos_;
    bool enabled_;
    int lower_index_;
    int upper_index_;
    int max_index_;
    QSharedPointer<FTViewSettings> view_settings_;

    static int MAX_HISTORY_SIZE_;

    QMutex update_buffer_lock_;
};

} // TiAScope

#endif // FREQUENCY_SPECTRUM_GRAPHICS_OBJECT_H
