// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunction.h"
#include "./engine/AppManager.h"

int32 UMyBlueprintFunction::AppUpdate()
{

}

int32 UMyBlueprintFunction::disConnect()
{

}

int32 UMyBlueprintFunction::isSecurity()
{

}

int32 UMyBlueprintFunction::read_int8()
{

}

int32 UMyBlueprintFunction::read_int16()
{

}

int32 UMyBlueprintFunction::read_int32()
{

}

int32 UMyBlueprintFunction::read_float()
{

}

int32 UMyBlueprintFunction::read_bool()
{

}

FVector UMyBlueprintFunction::read_FVector()
{

}

FRotator UMyBlueprintFunction::read_FRotator()
{

}

FString UMyBlueprintFunction::read_FString_len(int32 len)
{

}

FString UMyBlueprintFunction::read_FString()
{

}

/************************************************************************/
/* 封包处理                                                                     */
/************************************************************************/

void UMyBlueprintFunction::send_begin(int32 cmd) //cmd 是两个字节
{
	app::__TcpClient->begin(cmd);
}

void UMyBlueprintFunction::send_end(int32 cmd)
{

}

void UMyBlueprintFunction::send_int8(int32 value)
{

}

void UMyBlueprintFunction::send_int16(int32 value)
{

}

void UMyBlueprintFunction::send_int32(int32 value)
{

}

void UMyBlueprintFunction::send_float(float value)
{

}

void UMyBlueprintFunction::send_bool(bool value)
{

}

void UMyBlueprintFunction::send_FVector(FVector value)
{

}

void UMyBlueprintFunction::send_FRotator(FRotator value)
{

}

void UMyBlueprintFunction::send_FString_len(FString value, int32 len)
{

}

void UMyBlueprintFunction::send_FString(FString value)
{

}
