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

//����ʱ����
void Atry01GameMode::StartGame() const
{
	
	AMyGameState* GetMyGameState = this->GetGameState<AMyGameState>();
	if (GetWorld()->GetGameInstance<UMyGameInstance>()->bHasSwitched) {
		GetMyGameState->StartCountDown();
	}
}


//��¼�߼���ÿ����Ұ���˳������
void Atry01GameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		// �����û��������ڼ���˳��
		FString PlayerName = FString::Printf(TEXT("Player_%d"), ++PlayerJoinIndex);
		PlayerState->SetPlayerName(PlayerName);

		// ��ӡ��־ȷ��
		UE_LOG(LogTemp, Log, TEXT("Player Joined: %s"), *PlayerName);
	}
}

//���׼���Լ��л��߼�����
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
			return; // �����δ׼����
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

		// ��ʼ��Ϸ�߼�
		UE_LOG(LogTemp, Log, TEXT("Game Started!"));

		// ��������ƶ�
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
