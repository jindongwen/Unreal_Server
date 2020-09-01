#include "../MyGameInstance.h"
#include "Misc/SecureHash.h"  //UE �?MD5 �?#13;
#include "TcpClient.h"

namespace net
{
//����
void TcpClient::parseCommand()
{
	if (socketfd < 0)
		return;
	auto c = getData();
	if (c->state < func::C_Connect)
		return;

	//����������
	onHeart();
	//����ָ��
	while (c->recv_Tail - c->recv_Head > 7)
	{
		// 1������ͷ
		uint8 head[2];
		head[0] = c->recvBuf[c->recv_Head] ^ c->rCode;
		head[1] = c->recvBuf[c->recv_Head + 1] ^ c->rCode;

		if (head[0] != func::__ClientInfo->Head[0] || head[1] != func::__ClientInfo->Head[1])
		{
			disconnectServer(2001, "head error...");
			return;
		}

		int32  cl  = (*( int32* )(c->recvBuf + c->recv_Head + 2)) ^ c->rCode;
		uint16 cmd = (*( uint16* )(c->recvBuf + c->recv_Head + 6)) ^ c->rCode;

		// 2�����Ȳ��� ��Ҫ�����ȴ�
		if (c->recv_Tail < c->recv_Head + cl)
			break;
		c->recv_TempHead = c->recv_Head + 8;
		c->recv_TempTail = c->recv_Head + cl;
		parseCommand(cmd);
		if (c->state < func::C_Connect)
			return;

		// 4�����Ӷ�ȡ����
		c->recv_Head += cl;
	}
	//��������
	this->onSend();
}
void TcpClient::onHeart()
{
	if (__AppGameInstance == nullptr)
		return;
	auto c = getData();
	if (c->state < func::C_ConnectSecure)
		return;

	if (m_data.time_HeartTime > __AppGameInstance->GetTimeSeconds() || m_data.time_HeartTime == 0)
		m_data.time_HeartTime = __AppGameInstance->GetTimeSeconds();

	int32 tempTime = __AppGameInstance->GetTimeSeconds() - m_data.time_HeartTime;
	if (tempTime >= func::__ClientInfo->HeartTime * 1000)
	{
		this->disconnectServer(7000, "on heart");
		return;
	}

	tempTime = __AppGameInstance->GetTimeSeconds() - m_data.time_Heart;

	// if ( tempTime >= func::__ClientInfo->HeartTime )
	if (tempTime >= 3000)
	{
		m_data.time_Heart = __AppGameInstance->GetTimeSeconds();
		begin(CMD_HEART);
		sss(1);
		end();
	}
}
void TcpClient::parseCommand(uint16 cmd)
{
	m_data.time_HeartTime = __AppGameInstance->GetTimeSeconds();
	if (cmd < 65000)
	{
		if (onCommand != nullptr)
			onCommand(this, cmd);
		return;
	}

	switch (cmd)
	{
	case CMD_RCODE:
	{
		auto c = getData();
		read(c->rCode);

		FString value = func::__ClientInfo->SafeCode + FString::Printf(TEXT("_%d"), c->rCode);
		c->md5        = FMD5::HashAnsiString(*value);  // TCHAR* = *FString
		// c->md5        = FMD5::HashAnsiString( value.GetCharArray().GetData() );  //
		TCHAR* pdata = c->md5.GetCharArray().GetData();
		uint8* md5   = ( uint8* )TCHAR_TO_UTF8(pdata);

		//����MD5��֤
		begin(CMD_SECURITY);
		sss(0);
		sss(( int8 )0);
		sss(func::__ClientInfo->Version);
		sss(md5, MAX_MD5_LEN);
		end();
	}
	break;
	case CMD_SECURITY:
	{
		auto   c    = getData();
		uint16 kind = 0;
		read(kind);
		if (kind > 0)
		{
			// 1 �汾���� 2 MD5����
			if (onExceptEvent != nullptr)
				onExceptEvent(this, kind);
			break;
		}

		c->state = func::C_ConnectSecure;
		if (onSecureEvent != nullptr)
			onSecureEvent(this, 0);
	}

	break;
	}
}
//**********************************************************************
//**********************************************************************
void TcpClient::begin(const uint16 cmd)
{
	auto c = getData();
	//ͷβ���
	if (c->send_Head == c->send_Tail)
	{
		c->send_Tail = 0;
		c->send_Head = 0;
	}
	c->send_TempTail = c->send_Tail;

	if (c->state >= func::C_Connect && c->is_Sending == false && socketfd > 0 && c->send_Tail + 8 <= func::__ClientInfo->SendMax)
	{
		c->is_Sending                = true;
		c->sendBuf[c->send_Tail + 0] = func::__ClientInfo->Head[0] ^ c->rCode;
		c->sendBuf[c->send_Tail + 1] = func::__ClientInfo->Head[1] ^ c->rCode;

		uint16 newcmd                = cmd ^ c->rCode;
		uint8* a                     = ( uint8* )&newcmd;
		c->sendBuf[c->send_Tail + 6] = a[0];
		c->sendBuf[c->send_Tail + 7] = a[1];

		c->send_TempTail += 8;
		return;
	}

	disconnectServer(6001, "b error...");
}

void TcpClient::end()
{
	auto c = getData();
	if (c->state == func::C_Free || c->is_Sending == false || socketfd < 0 || c->send_Tail + 8 > func::__ClientInfo->SendMax || c->send_TempTail > func::__ClientInfo->SendMax
	    || c->send_Tail >= c->send_TempTail)
	{
		disconnectServer(6002, "e error...");
		return;
	}

	c->is_Sending                = false;
	uint32 len                   = (c->send_TempTail - c->send_Tail) ^ c->rCode;
	uint8* a                     = ( uint8* )&len;
	c->sendBuf[c->send_Tail + 2] = a[0];
	c->sendBuf[c->send_Tail + 3] = a[1];
	c->sendBuf[c->send_Tail + 4] = a[2];
	c->sendBuf[c->send_Tail + 5] = a[3];

	//��������ֵ
	c->send_Tail = c->send_TempTail;
}

void TcpClient::sss(const int8 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 1 < func::__ClientInfo->SendMax)
	{
		c->sendBuf[c->send_TempTail] = v;
		c->send_TempTail++;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const uint8 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 1 < func::__ClientInfo->SendMax)
	{
		c->sendBuf[c->send_TempTail] = v;
		c->send_TempTail++;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const int16 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 2 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 2; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 2;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const uint16 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 2 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 2; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 2;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const int32 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 4 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 4; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 4;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const uint32 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 4 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 4; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 4;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const int64 v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 8 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 8; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 8;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const bool v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 1 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;

		c->sendBuf[c->send_TempTail] = v;
		c->send_TempTail += 1;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const float v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 4 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 4; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 4;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(const double v)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + 8 < func::__ClientInfo->SendMax)
	{
		uint8* p = ( uint8* )&v;
		for (int i = 0; i < 8; i++)
			c->sendBuf[c->send_TempTail + i] = p[i];

		c->send_TempTail += 8;
		return;
	}

	c->is_Sending = false;
}

void TcpClient::sss(void* v, const int32 len)
{
	auto c = getData();
	if (c->is_Sending && c->send_TempTail + len < func::__ClientInfo->SendMax)
	{
		memcpy(&c->sendBuf[c->send_TempTail], v, len);

		c->send_TempTail += len;
		return;
	}
	c->is_Sending = false;
}
//*******************************************************************
//*******************************************************************
//*******************************************************************
bool isValid(S_SERVER_BASE* c, int32 value)
{
	if (c->state == func::C_Free || c->recv_TempTail == 0 || c->recv_TempHead + value > c->recv_TempTail)
	{
		return false;
	}
	return true;
}

void TcpClient::read(int8& v)
{
	auto c = getData();
	if (isValid(c, 1) == false)
	{
		v = 0;
		return;
	}
	v = (*( int8* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead++;
}

void TcpClient::read(uint8& v)
{
	auto c = getData();
	if (isValid(c, 1) == false)
	{
		v = 0;
		return;
	}
	v = (*( uint8* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead++;
}

void TcpClient::read(int16& v)
{
	auto c = getData();
	if (isValid(c, 1) == false)
	{
		v = 0;
		return;
	}
	v = (*( int16* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 2;
}

void TcpClient::read(uint16& v)
{
	auto c = getData();
	if (isValid(c, 2) == false)
	{
		v = 0;
		return;
	}
	v = (*( uint16* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 2;
}

void TcpClient::read(int32& v)
{
	auto c = getData();
	if (isValid(c, 4) == false)
	{
		v = 0;
		return;
	}
	v = (*( int32* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 4;
}

void TcpClient::read(uint32& v)
{
	auto c = getData();
	if (isValid(c, 4) == false)
	{
		v = 0;
		return;
	}
	v = (*( uint32* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 4;
}

void TcpClient::read(int64& v)
{
	auto c = getData();
	if (isValid(c, 8) == false)
	{
		v = 0;
		return;
	}
	v = (*( int64* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 8;
}

void TcpClient::read(bool& v)
{
	auto c = getData();
	if (isValid(c, 1) == false)
	{
		v = 0;
		return;
	}
	v = (*( bool* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 1;
}

void TcpClient::read(float& v)
{
	auto c = getData();
	if (isValid(c, 4) == false)
	{
		v = 0;
		return;
	}
	v = (*( float* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 4;
}

void TcpClient::read(double& v)
{
	auto c = getData();
	if (isValid(c, 8) == false)
	{
		v = 0;
		return;
	}
	v = (*( double* )(c->recvBuf + c->recv_TempHead));
	c->recv_TempHead += 8;
}

void TcpClient::read(void* v, const int32 len)
{
	auto c = getData();
	if (isValid(c, len) == false)
	{
		v = 0;
		return;
	}
	memcpy(v, &c->recvBuf[c->recv_TempHead], len);
	c->recv_TempHead += len;
}

void TcpClient::setOnConnect(TCPCLIENTNOTIFY_EVENT event)
{
	onAcceptEvent = event;
}

void TcpClient::setOnConnectSecure(TCPCLIENTNOTIFY_EVENT event)
{
	onSecureEvent = event;
}

void TcpClient::setOnDisconnect(TCPCLIENTNOTIFY_EVENT event)
{
	onDisconnectEvent = event;
}

void TcpClient::setOnExcept(TCPCLIENTNOTIFY_EVENT event)
{
	onExceptEvent = event;
}

void TcpClient::setOnCommand(TCPCLIENTNOTIFY_EVENT event)
{
	onCommand = event;
}

}  // namespace net
