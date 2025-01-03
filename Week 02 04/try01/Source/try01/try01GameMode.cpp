// Copyright Epic Games, Inc. All Rights Reserved.

#include "try01GameMode.h"
#include "try01Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerState.h"
#include "MyGameState.h"
#include "try01PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

Atry01GameMode::Atry01GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	PlayerStateClass = AMyPlayerState::StaticClass();
	
	GameStateClass = AMyGameState::StaticClass();

	
	PlayerJoinIndex = 0;
}

void Atry01GameMode::BeginPlay(){
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("start game"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Start game"));
	StartGame();
}

//倒计时部分
void Atry01GameMode::StartGame() const
{
	
	AMyGameState* GetMyGameState = this->GetGameState<AMyGameState>();
	if (GetWorld()->GetGameInstance<UMyGameInstance>()->bHasSwitched) {
		GetMyGameState->StartCountDown();
	}
}


//登录逻辑，每个玩家按照顺序命名
void Atry01GameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		// 分配用户名，基于加入顺序
		FString PlayerName = FString::Printf(TEXT("Player_%d"), ++PlayerJoinIndex);
		PlayerState->SetPlayerName(PlayerName);

		// 打印日志确认
		UE_LOG(LogTemp, Log, TEXT("Player Joined: %s"), *PlayerName);
	}
}

//玩家准备以及切换逻辑部分
void Atry01GameMode::SwitchToResetLevel() const
{
	GetWorld()->ServerTravel(TEXT("/Game/FirstPerson/Maps/FightMap"), false);

}


void Atry01GameMode::CheckAllPlayersReady()
{
	for (APlayerState* PlayerState : GameState->PlayerArray) 
	{
		AMyPlayerState* CustomPlayerState = Cast<AMyPlayerState>(PlayerState);
		if (CustomPlayerState && !CustomPlayerState->bIsReady)
		{
			return; // 有玩家未准备好
		}
	}
	StartCountdown();
}


void Atry01GameMode::StartCountdown()
{
	CountdownTime = 5;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		Atry01PlayerController* PlayerController = Cast<Atry01PlayerController>(*It);
		if (PlayerController && HasAuthority())
		{
			PlayerController->SetPlayerEnabledState(false);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &Atry01GameMode::CountdownTick, 1.0f, true);
}

void Atry01GameMode::CountdownTick()
{
	if (CountdownTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

		// 开始游戏逻辑
		UE_LOG(LogTemp, Log, TEXT("Game Started!"));

		// 允许玩家移动
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			Atry01PlayerController* PlayerController = Cast<Atry01PlayerController>(*It);
			if (PlayerController)
			{
				PlayerController->SetPlayerEnabledState(true);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("Game Started!"));
		GetWorld()->GetGameInstance<UMyGameInstance>()->bHasSwitched = true;
		SwitchToResetLevel();

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game starts in %d seconds"), CountdownTime);
		CountdownTime--;
	}
}
