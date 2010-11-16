#include "signal_graphics_scene.h"
#include "signal_graphics_object.h"
#include "base/helpers.h"

namespace tobiss { namespace scope {

//-------------------------------------------------------------------------------------------------
SignalGraphicsScene::SignalGraphicsScene (QObject *parent) :
    QGraphicsScene (0, 0, 0, 0, parent)
{
    setObjectName (QString("SignalGraphicsScene - 0x").append(QString::number(reinterpret_cast<unsigned>(this), 16)));
}

//-------------------------------------------------------------------------------------------------
void SignalGraphicsScene::addSignalGraphicsObject (SignalTypeFlag signal_name, SignalGraphicsObject* signal)
{
    int y_pos = 0;
    if (signals_.size() > 0)
        y_pos = signals_.last()->pos().y() + signals_.last()->boundingRect().height();

    signal_map_[signal_name] = signal;
    signal_list_index_[signal] = signals_.size();
    if (signals_.size())
    {
        signal->connect(signals_.last(), SIGNAL(bottomYChanged (int)), SLOT(setYPos(int)));
        disconnect (signals_.last(), 0, this, 0);
    }
    connect (signal, SIGNAL(bottomYChanged(int)), SLOT(setSceneRectHeight(int)));
    signals_.push_back (signal);
    signal->setPos (0, y_pos);
    signal->connect (&signals_timer_, SIGNAL(timeout()), SLOT(updateToDataBuffer()));

    setSceneRect (0, 0, 850, y_pos + signal->boundingRect().height());

    this->addItem (signal);

}

//-------------------------------------------------------------------------------------------------
void SignalGraphicsScene::setSignalVisibility (SignalTypeFlag signal_name, bool visibility)
{
    QRectF scene_rect = sceneRect ();
    SignalGraphicsObject* signal = signal_map_[signal_name];
    if (!visibility)
    {
        scene_rect.setHeight (scene_rect.height() - signal->height());
        helpers::animateProperty (signal, "height", signal->height(), 0, signal, SLOT(setVisibleFalse()));
    }
    else
    {
        signal->setVisibleTrue();
        scene_rect.setHeight (scene_rect.height() + signal->defaultHeight());
        helpers::animateProperty (signal, "height", 0, signal->defaultHeight());
    }
    helpers::animateProperty (this, "sceneRect", sceneRect(), scene_rect);
}

//-------------------------------------------------------------------------------------------------
void SignalGraphicsScene::startTimer (int milli_seconds_interval)
{
    signals_timer_.setInterval (milli_seconds_interval);
    signals_timer_.start ();
}

//-------------------------------------------------------------------------------------------------
void SignalGraphicsScene::setSceneRectHeight (int height)
{
    setSceneRect (sceneRect().x(), sceneRect().y(), sceneRect().width(), height);
}

//---------------------------------------------------------------------------------------------
void SignalGraphicsScene::setSceneRectWidth (int width)
{
    setSceneRect (sceneRect().x(), sceneRect().y(), width, sceneRect().height());
}



} } // namespace
