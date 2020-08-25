#ifndef __IDEFINE_H
#define __IDEFINE_H

#include "Chaos/Defines.h"
//
#define MAX_MD5_LEN 35

#define CMD_HEART 60000
#define CMD_RCODE 65001
#define CMD_SECURITY 65002

namespace func
{
enum C_SOCKET_STATE
{
    C_Free          = 0,
    C_ConnectTry    = 1,
    C_Connect       = 2,
    C_ConnectSecure = 3,
    C_Login         = 4
};
struct ConfigXML
{
    int32   ID;    //服务器ID
    uint8   Type;  //服务器类型 1-DB 2-中心服务器 3-地图服务器 4-网关服务器
    uint8   RCode;
    int32   Version;
    int32   ReceOne;
    int32   ReceMax;
    int32   SendOne;
    int32   SendMax;
    int32   HeartTime;  //多少秒没收到服务端的心跳包..
    int32   AutoTime;   //自动重连时间
    FString SafeCode;
    uint8   Head[ 2 ];
};

extern ConfigXML* __ClientInfo;

}  // namespace func

namespace net
{
struct S_SERVER_BASE
{
    int32   ID;  //客户端ID
    FString ip;
    uint16  port;
    int32   serverID;    // 0 1000-DB 2000-Center 3000-Game 4000-Gate 5000-Login
    uint8   serverType;  // 0 1-DB 2-Center 3-Game 4-Gate 5-Login

    uint8 state;  //用户状态 0空闲 1尝试连接 2安全连接  3连接成功
    uint8 rCode;

    uint8* recvBuf_Temp;
    uint8* recvBuf;
    int32  recv_Head;      //头 消费者使用
    int32  recv_Tail;      //尾 生产者使用
    int32  recv_TempHead;  //临时头
    int32  recv_TempTail;  //临时尾
    bool   is_Recved;

    uint8* sendBuf;
    int32  send_Head;
    int32  send_Tail;
    int32  send_TempTail;
    bool   is_Sending;
    bool   is_SendCompleted;

    int32   time_Heart;      //上次发送心跳包的时间
    int32   time_HeartTime;  //接收心跳包
    int32   time_AutoConnect;
    FString md5;

    void init( int sid );
    void reset();
};

}  // namespace net
#endif
