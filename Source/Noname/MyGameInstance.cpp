// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Engine/World.h"
#include "./engine/AppManager.h"
#include "Engine/GameInstance.h"

UMyGameInstance* __AppGameInstance = nullptr;

int UMyGameInstance::AppInitGameInstance()
{
    auto www          = GetWorld();
    __AppGameInstance = Cast< UMyGameInstance >( GetWorld()->GetGameInstance() );
    app::run();
    return 0;
}

void UMyGameInstance::Shutdown()
{
    if (app::__TcpClient != NULL)
    {
        app::__TcpClient->setThread(true); //设置是否暂停
        app::__TcpClient->disconnectServer(8000, "shutdown");
    }
    Super::Shutdown();
}

int32 UMyGameInstance::GetTimeSeconds()
{
    int32 ftime = GetWorld()->GetTimeSeconds() * 1000;  //毫秒 游戏开始到现在的时间
    return ftime;
}
