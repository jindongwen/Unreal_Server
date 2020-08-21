#include "HAL/UnrealMemory.h"
#include "TcpClient.h"

namespace func
{
ConfigXML* __ClientInfo = nullptr;

}  // namespace func

namespace net
{

void S_SERVER_BASE::init(int sid)
{
	ID           = 0;
	serverID     = sid;
	serverType   = 0;
	recvBuf      = ( uint8* )FMemory::Malloc[func::__ClientInfo->ReceMax];
	sendBuf      = ( uint8* )FMemory::Malloc[func::__ClientInfo->SendMax];
	recvBuf_Temp = ( uint8* )FMemory::Malloc[func::__ClientInfo->ReceOne];

	port = 0;
	memset(ip, 0, 16);
	reset();
}
void S_SERVER_BASE::reset()
{
	state         = 0;
	rCode         = func::__ClientInfo->RCode;
	recv_Head     = 0;
	recv_Tail     = 0;
	recv_TempHead = 0;
	recv_TempTail = 0;
	is_Recved     = false;

	send_Head        = 0;
	send_Tail        = 0;
	send_TempTail    = 0;
	is_Sending       = false;
	is_SendCompleted = false;
	time_Heart       = ( int )time(NULL);
	time_AutoConnect = ( int )time(NULL);

	memset(recvBuf, 0, func::__ClientInfo->ReceMax);
	memset(sendBuf, 0, func::__ClientInfo->SendMax);
	memset(recvBuf_Temp, 0, func::__ClientInfo->ReceOne);
}
}  // namespace net

int32 net::TcpClient::initSocket()
{
	return int32();
}

int32 net::TcpClient::onRecv() {}

int32 net::TcpClient::onSend() {}
