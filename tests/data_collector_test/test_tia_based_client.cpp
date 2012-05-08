#include <QtCore/QString>


#include "../UnitTest++/UnitTest++.h"


#include "../../src/data_collector/qt_tia_client/impl/tia_datapacket_basedlibtia.h"
#include "../../src/data_collector/qt_tia_client/impl/tia_client_based_libtia.h"


#include "tia/defines.h"
#include "tia/constants.h"

#include "print_to_console.h"

using namespace TiAQtImplementation;

//-----------------------------------------------------------------------------

//TEST(toUpper)

//{
//    QString str = "Hello";

//    CHECK(str.toUpper() == QString("HELLO"));
//}

//-----------------------------------------------------------------------------

TEST(tryConnectoToServer)
{
    TiAQtClientBasedLibTiA client (true);

    client.connectToServer("127.0.0.1",9000,false);

    CHECK(client.connected());

    quint32 loops = 5;

    client.startReceiving();

    while(loops--)
    {
        QSharedPointer<DataPacket> packet = client.getDataPacket();

        printDataPacketToConsole(packet);
     }

    client.disconnectFromServer();

    CHECK(!client.connected());
}
