#ifndef ____TCPCLIENT_H
#define ____TCPCLIENT_H

#include "Engine/Engine.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "IDefine.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
namespace net
{
class TcpClient;
typedef void ( *TCPCLIENTNOTIFY_EVENT )( TcpClient* tcp, const int32 code );

class TcpClient_Thread : public FRunnable
{
public:
    TcpClient_Thread( TcpClient* c );
    virtual ~TcpClient_Thread();
    virtual uint32 Run();  //纯虚函数必须实现
    virtual void   Exit();

    void StopThread();

    FRunnableThread* thread;

    TcpClient* tcp;
};

class TcpClient
{
private:
    bool isFirstConnect;  //自动连接使用
    bool isRunning;
    bool isPause;

    FSocket*          socketfd;
    TcpClient_Thread* m_workthread;
    S_SERVER_BASE     m_data;
    //	std::shared_ptr<std::thread> m_workthread;

    TCPCLIENTNOTIFY_EVENT onAcceptEvent;
    TCPCLIENTNOTIFY_EVENT onSecureEvent;
    TCPCLIENTNOTIFY_EVENT onDisconnectEvent;
    TCPCLIENTNOTIFY_EVENT onExceptEvent;
    TCPCLIENTNOTIFY_EVENT onCommand;

    int32 initSocket();
    int32 onRecv();
    int32 onSend();

    void onHeart();
    void parseCommand( uint16 cmd );

public:
    TcpClient();
    virtual ~TcpClient();
    virtual inline S_SERVER_BASE* getData()
    {
        return &m_data;
    };

    void run();
    void stop();
    void setThread( bool ispause );

    void runClient( uint32 sid, FString ip, int32 port );
    bool connectServer();
    void disconnectServer( const int32 errcode, FString err );

    void begin( const uint16 cmd );
    void end();
    void sss( const int8 v );
    void sss( const uint8 v );
    void sss( const int16 v );
    void sss( const uint16 v );
    void sss( const int32 v );
    void sss( const uint32 v );
    void sss( const int64 v );
    void sss( const bool v );
    void sss( const float v );
    void sss( const double v );
    void sss( void* v, const int32 len );

    void read( int8& v );
    void read( uint8& v );
    void read( int16& v );
    void read( uint16& v );
    void read( int32& v );
    void read( uint32& v );
    void read( int64& v );
    void read( bool& v );
    void read( float& v );
    void read( double& v );
    void read( void* v, const int32 len );

    void parseCommand();
    void setOnConnect( TCPCLIENTNOTIFY_EVENT event );
    void setOnConnectSecure( TCPCLIENTNOTIFY_EVENT event );
    void setOnDisconnect( TCPCLIENTNOTIFY_EVENT event );
    void setOnExcept( TCPCLIENTNOTIFY_EVENT event );
    void setOnCommand( TCPCLIENTNOTIFY_EVENT event );
};

}  // namespace net

#endif
