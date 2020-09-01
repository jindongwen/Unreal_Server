// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 *
 */
UCLASS()
class NONAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "MyEngine")
	int AppInitGameInstance();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyEngine")
	void onCommand(int cmd);
	UFUNCTION(BlueprintImplementableEvent, Category = "MyEngine")
	void onConnect(int errcode);
	UFUNCTION(BlueprintImplementableEvent, Category = "MyEngine")
	void onSecurity(int errcode);
	UFUNCTION(BlueprintImplementableEvent, Category = "MyEngine")
	void onDisconnect(int errcode);
	UFUNCTION(BlueprintImplementableEvent, Category = "MyEngine")
	void onExcept(int errcode);

	virtual void Shutdown();

	int32 GetTimeSeconds();
};

extern UMyGameInstance* __AppGameInstance;
