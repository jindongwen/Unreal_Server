#include "TcpClient.h"
#include "HAL/UnrealMemory.h"
#include "IDefine.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"

namespace func
{
ConfigXML* __ClientInfo = nullptr;
}  // namespace func

namespace net
{

void S_SERVER_BASE::init( int sid )
{
    ID           = 0;
    serverID     = sid;
    serverType   = 0;
    recvBuf      = ( uint8* )FMemory::Malloc( func::__ClientInfo->ReceMax );
    sendBuf      = ( uint8* )FMemory::Malloc( func::__ClientInfo->SendMax );
    recvBuf_Temp = ( uint8* )FMemory::Malloc( func::__ClientInfo->ReceOne );
    ip           = "192.144.219.120";
    port         = 13550;

    port = 0;
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
    time_Heart       = 0;
    time_AutoConnect = 0;

    FMemory::Memset( recvBuf, 0, sizeof( func::__ClientInfo->ReceMax ) );

    memset( recvBuf, 0, func::__ClientInfo->ReceMax );
    memset( sendBuf, 0, func::__ClientInfo->SendMax );
    memset( recvBuf_Temp, 0, func::__ClientInfo->ReceOne );
}

}  // namespace net

net::TcpClient::TcpClient()
{
    isPause        = false;
    isFirstConnect = true;
    isRunning      = false;
    socketfd       = nullptr;

    onAcceptEvent     = nullptr;
    onSecureEvent     = nullptr;
    onDisconnectEvent = nullptr;
    onExceptEvent     = nullptr;
    onCommand         = nullptr;

    m_workthread = nullptr;
    socketfd     = nullptr;
}

net::TcpClient::~TcpClient()
{
    if ( m_workthread != nullptr )
    {
        delete m_workthread;
        m_workthread = nullptr;
    }
    if ( socketfd != nullptr )
    {
        ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->DestroySocket( socketfd );
        socketfd = nullptr;
    }
    m_data.reset();
}

int32 net::TcpClient::initSocket()
{
    if ( socketfd != nullptr )
    {
        // socket 关闭
        ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->DestroySocket( socketfd );  //销毁socket
        socketfd = nullptr;
    }
    //创建 socket
    socketfd = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateSocket( NAME_Stream, TEXT( "flower" ), false );
    return 0;
}
bool net::TcpClient::connectServer()
{
    if ( m_data.state >= func::C_Connect )
        return false;
    initSocket();
    if ( socketfd == nullptr )
        return false;

    FIPv4Address ip;
    FIPv4Address::Parse( m_data.ip, ip );  // inet_addr

    //相当于 sockaddr_in ?
    TSharedRef< FInternetAddr > addr = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateInternetAddr();
    addr->SetIp( ip.Value );
    addr->SetPort( m_data.port );

    //连接服务器
    bool isconnet = socketfd->Connect( *addr );
    if ( isconnet )
    {
        //设置非阻塞
        socketfd->SetNonBlocking();
        m_data.state = func::C_Connect;

        m_data.time_HeartTime = 0;
        if ( onAcceptEvent != nullptr )
            onAcceptEvent( this, 0 );
        return true;
    }
    return false;
}

void net::TcpClient::runClient( uint32 sid, FString ip, int32 port )
{
    m_data.init( sid );
    m_data.time_AutoConnect = 0;
    m_data.ip               = ip;
    m_data.port             = port;
    isRunning               = true;
    m_workthread            = new TcpClient_Thread( this );
}

void net::TcpClient::disconnectServer( const int32 errcode, FString err )
{
    if ( m_data.state == func::C_Free )
        return;
    if ( socketfd == nullptr )
        return;
    socketfd->Close();
    m_data.reset();

    if ( onDisconnectEvent )
        onDisconnectEvent( this, errcode );
}

/************************************************************************/
/*  发送 接收数据                                                                     */
/************************************************************************/
int32 net::TcpClient::onRecv()
{
    if ( socketfd == nullptr )
        return -1;
    FMemory::Memset( m_data.recvBuf, 0, func::__ClientInfo->ReceOne );
    uint32 size;
    if ( socketfd->HasPendingData( size ) == false )
        return -1;

    int32 recvBytes = 0;
    bool  isrecv    = socketfd->Recv( m_data.recvBuf_Temp, func::__ClientInfo->ReceOne, recvBytes );
    if ( isrecv && recvBytes > 0 )
    {
        auto c = this->getData();
        if ( c->recv_Tail == c->recv_Head )
        {
            c->recv_Tail = 0;
            c->recv_Head = 0;
        }

        if ( c->recv_Tail + recvBytes > func::__ClientInfo->ReceOne )
            return -1;
        FMemory::Memcpy( &c->recvBuf[ c->recv_Tail ], c->recvBuf, recvBytes );
        c->recv_Tail += recvBytes;
    }
    return 0;
}

int32 net::TcpClient::onSend()
{
    auto c = this->getData();
    if ( c->send_Tail <= c->send_Head )
        return 0;
    if ( c->state < func::C_Connect )
        return -1;
    int32 sendlen = c->send_Tail - c->send_Head;
    if ( sendlen < 1 )
        return 0;

    int32 sendBytes = 0;
    bool  issend    = socketfd->Send( &c->sendBuf[ c->send_Head ], sendlen, sendBytes );
    if ( issend && sendBytes > 0 )
    {
        c->send_Head += sendBytes;
    }
    return 0;
}
