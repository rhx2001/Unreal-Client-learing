// Copyright Epic Games, Inc. All Rights Reserved.


#include "try01PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MyPlayerState.h"
#include "try01GameMode.h"
#include "EnhancedInputComponent.h"

void Atry01PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	

}

void Atry01PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 确保 InputComponent 是 EnhancedInputComponent
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 绑定 Ready 输入到函数
		EnhancedInput->BindAction(ReadyInputAction, ETriggerEvent::Started, this, &Atry01PlayerController::HandleReadyInput);
	}
}

void Atry01PlayerController::HandleReadyInput(const FInputActionValue& Value)
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	if (MyPlayerState && !MyPlayerState->bIsReady)
	{
		ServerReady();
	}
}


void Atry01PlayerController::ServerReady_Implementation() {
	AMyPlayerState* MyplayerState = GetPlayerState<AMyPlayerState>();
	if (MyplayerState) {
		MyplayerState->bIsReady = true;
		UE_LOG(LogTemp, Log, TEXT("Player %s is ready!"), *MyplayerState->GetPlayerName());
	}
	Atry01GameMode* GameMode = Cast<Atry01GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->CheckAllPlayersReady();
	}
}

void Atry01PlayerController::SetPlayerEnabledState_Implementation(bool bEnabled)
{
	if (bEnabled)
	{
		EnableInput(this);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
		

	}
	else
	{
		DisableInput(this);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}
