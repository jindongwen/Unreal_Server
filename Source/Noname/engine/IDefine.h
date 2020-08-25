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
    int32   ID;    //������ID
    uint8   Type;  //���������� 1-DB 2-���ķ����� 3-��ͼ������ 4-���ط�����
    uint8   RCode;
    int32   Version;
    int32   ReceOne;
    int32   ReceMax;
    int32   SendOne;
    int32   SendMax;
    int32   HeartTime;  //������û�յ�����˵�������..
    int32   AutoTime;   //�Զ�����ʱ��
    FString SafeCode;
    uint8   Head[ 2 ];
};

extern ConfigXML* __ClientInfo;

}  // namespace func

namespace net
{
struct S_SERVER_BASE
{
    int32   ID;  //�ͻ���ID
    FString ip;
    uint16  port;
    int32   serverID;    // 0 1000-DB 2000-Center 3000-Game 4000-Gate 5000-Login
    uint8   serverType;  // 0 1-DB 2-Center 3-Game 4-Gate 5-Login

    uint8 state;  //�û�״̬ 0���� 1�������� 2��ȫ����  3���ӳɹ�
    uint8 rCode;

    uint8* recvBuf_Temp;
    uint8* recvBuf;
    int32  recv_Head;      //ͷ ������ʹ��
    int32  recv_Tail;      //β ������ʹ��
    int32  recv_TempHead;  //��ʱͷ
    int32  recv_TempTail;  //��ʱβ
    bool   is_Recved;

    uint8* sendBuf;
    int32  send_Head;
    int32  send_Tail;
    int32  send_TempTail;
    bool   is_Sending;
    bool   is_SendCompleted;

    int32   time_Heart;      //�ϴη�����������ʱ��
    int32   time_HeartTime;  //����������
    int32   time_AutoConnect;
    FString md5;

    void init( int sid );
    void reset();
};

}  // namespace net
#endif
