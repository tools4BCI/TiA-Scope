#ifndef LINE_RECEIVE_BLOCKER_H
#define LINE_RECEIVE_BLOCKER_H

#include "../tia_exception.h"

#include <QObject>
#include <QTcpSocket>
#include <QWaitCondition>
#include <QMutex>
#include <QStringList>
#include <QDebug>

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
class LineReceiveBlocker : public QObject
{
    Q_OBJECT
public:
    //-------------------------------------------------------------------------
    LineReceiveBlocker (QTcpSocket& text_socket) : text_socket_ (text_socket)
    {
        //this->connect (&text_socket_, SIGN);
        //qDebug () << __FUNCTION__ << this->connect (&text_socket_, SIGNAL(readyRead()), SLOT(receiveData()));
    }


    //-------------------------------------------------------------------------
private Q_SLOTS:
    void receiveText ()
    {

    }

private:
    QTcpSocket& text_socket_;
};

}

#endif // LINE_RECEIVE_BLOCKER_H
