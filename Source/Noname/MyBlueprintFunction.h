// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunction.generated.h"

/**
 * 
 */
UCLASS()
class NONAME_API UMyBlueprintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "FlowerEngine")
		static int32 AppUpdate();
	UFUNCTION(BlueprintCallable, Category = "FlowerEngine")
		static int32 disConnect();
	UFUNCTION(BlueprintCallable, Category = "FlowerEngine")
		static int32 isSecurity();
	//BlueprintPure 用来返回一个或者多个值,也就是必须有返回值,或者参数是引用或指针
	//解析按一个字节解析,蓝图里只能按4个字节来读
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static int32 read_int8();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static int32 read_int16();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static int32 read_int32();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static int32 read_float();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static int32 read_bool();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static FVector read_FVector();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static FRotator read_FRotator();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static FString read_FString_len(int32 len);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FlowerEngine")
		static FString read_FString();

	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_begin(int32 cmd);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_end(int32 cmd);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_int8(int32 value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_int16(int32 value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_int32(int32 value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_float(float value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_bool(bool value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_FVector(FVector value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_FRotator(FRotator value);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_FString_len(FString value,int32 len);
	UFUNCTION(BlueprintCallable,Category = "FlowerEngine")
		static void send_FString(FString value);

	
};
