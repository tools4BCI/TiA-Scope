#ifndef SIGNAL_GRAPHICS_SCENE_H
#define SIGNAL_GRAPHICS_SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QString>
#include <QHash>

namespace tobiss { namespace scope {

class SignalGraphicsObject;

//-------------------------------------------------------------------------------------------------
class SignalGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    //---------------------------------------------------------------------------------------------
    explicit SignalGraphicsScene(QObject *parent = 0);

    //---------------------------------------------------------------------------------------------
    void addSignalGraphicsObject (QString const& signal_name, SignalGraphicsObject* signal);


public Q_SLOTS:
    //---------------------------------------------------------------------------------------------
    void setSignalVisibility (QString const& signal, bool visibility);

    //---------------------------------------------------------------------------------------------
    void startTimer (int milli_seconds_interval);

private:
    QList<SignalGraphicsObject*> signals_;
    QHash<SignalGraphicsObject*, int> signal_list_index_;
    QTimer signals_timer_;
    QHash<QString, SignalGraphicsObject*> signal_map_;

};

} } // namespace

#endif // SIGNAL_GRAPHICS_SCENE_H
