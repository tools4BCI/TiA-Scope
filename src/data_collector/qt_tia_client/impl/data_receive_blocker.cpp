#include "data_receive_blocker.h"

namespace TiAQtImplementation
{

//-----------------------------------------------------------------------------
void DataReceiveBlocker::receiveData ()
{
    data_stream_data_.append(data_socket_.readAll());
    data_stream_data_wait_.wakeAll ();
}

}
