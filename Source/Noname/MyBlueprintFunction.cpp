// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunction.h"
#include "./engine/AppManager.h"

int32 UMyBlueprintFunction::AppUpdate()
{
	app::onUpdate();
	return 0;
}

int32 UMyBlueprintFunction::disConnect()
{
	if (app::__TcpClient == NULL)
		return -1;
	app::__TcpClient->disconnectServer(6000, "blue close");
	return true;
}

bool UMyBlueprintFunction::isSecurity()
{
	if (app::__TcpClient == NULL)
		return false;
	uint8 state = app::__TcpClient->getData()->state;
	if (state < func::C_ConnectSecure)
		return false;
	return true;
}

int32 UMyBlueprintFunction::read_int8()
{
	int8 temp = 0;
	app::__TcpClient->read(temp);
	return temp;
}

int32 UMyBlueprintFunction::read_int16()
{
	uint16 temp = 0;
	app::__TcpClient->read(temp);
	return temp;
}

int32 UMyBlueprintFunction::read_int32()
{
	uint32 temp = 0;
	app::__TcpClient->read(temp);
	return temp;
}

float UMyBlueprintFunction::read_float()
{
	float temp = 0;
	app::__TcpClient->read(temp);
	return temp;
}

bool UMyBlueprintFunction::read_bool()
{
	bool temp = 0;
	app::__TcpClient->read(temp);
	return temp;
}

FVector UMyBlueprintFunction::read_FVector()
{
	FVector temp;
	app::__TcpClient->read(&temp, sizeof(FVector));
	return temp;
}

FRotator UMyBlueprintFunction::read_FRotator()
{
	FRotator temp;
	app::__TcpClient->read(&temp, sizeof(FVector));
	return temp;
}

FString UMyBlueprintFunction::read_FString_len(int32 len)
{
	uint8* cc = ( uint8* )FMemory::Malloc(len);
	app::__TcpClient->read(cc, len);
	FString value = UTF8_TO_TCHAR(cc);
	FMemory::Free(cc);
	return value;
}

FString UMyBlueprintFunction::read_FString()
{
	int len = 0;
	app::__TcpClient->read(len);
	if (len < 1 || len > 1024 * 1024 * 5)
		return "";
	uint8* cc = ( uint8* )FMemory::Malloc(len);
	app::__TcpClient->read(cc, len);
	FString value = UTF8_TO_TCHAR(cc);
	FMemory::Free(cc);
	return value;
}

FPlayerBase UMyBlueprintFunction::read_PlayerBase()
{
	FPlayerBase data;
	app::__TcpClient->read(&data, 48);

	FString nick;
	data.nick = read_FString_len(20);
	return data;
}

/************************************************************************/
/* 封包处理                                                                     */
/************************************************************************/

void UMyBlueprintFunction::send_begin(int32 cmd)  // cmd 是两个字节
{
	app::__TcpClient->begin(cmd);
}

void UMyBlueprintFunction::send_end(int32 cmd)
{
	app::__TcpClient->end();
}

void UMyBlueprintFunction::send_int8(int32 value)
{
	app::__TcpClient->sss(( int8 )value);
}

void UMyBlueprintFunction::send_int16(int32 value)
{
	app::__TcpClient->sss(( int16 )value);
}

void UMyBlueprintFunction::send_int32(int32 value)
{
	app::__TcpClient->sss(value);
}

void UMyBlueprintFunction::send_float(float value)
{
	app::__TcpClient->sss(value);
}

void UMyBlueprintFunction::send_bool(bool value)
{
	app::__TcpClient->sss(value);
}

void UMyBlueprintFunction::send_FVector(FVector value)
{
	app::__TcpClient->sss(&value, sizeof(FVector));
}

void UMyBlueprintFunction::send_FRotator(FRotator value)
{
	app::__TcpClient->sss(&value, sizeof(FVector));
}

void UMyBlueprintFunction::send_FString_len(FString value, int32 len)
{
	TCHAR* pdata = value.GetCharArray().GetData();
	uint8* cc    = ( uint8* )TCHAR_TO_UTF8(pdata);

	int size = value.GetCharArray().Num();
	if (size > len)
		size = len;

	app::__TcpClient->sss(cc, size);
	if (size == len)
		return;
	int    a   = size - len;
	uint8* cc2 = ( uint8* )FMemory::Malloc(a);
	app::__TcpClient->sss(cc2, a);
	FMemory::Free(cc2);
}

void UMyBlueprintFunction::send_FString(FString value)
{
	TCHAR* pdata = value.GetCharArray().GetData();
	uint8* cc    = ( uint8* )TCHAR_TO_UTF8(pdata);

	int size = value.GetCharArray().Num();

	app::__TcpClient->sss(size);
	app::__TcpClient->sss(cc, size);
}
