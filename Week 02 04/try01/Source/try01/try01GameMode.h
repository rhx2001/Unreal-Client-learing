// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "try01GameMode.generated.h"

UCLASS(minimalapi)
class Atry01GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	Atry01GameMode();

	uint32 PlayerJoinIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxImportantCubes = 5;

	int32 CurrentImportantCubes = 0;

	

public:
	void CheckAllPlayersReady();

	void PostLogin(APlayerController* NewPlayer);

	void SwitchToResetLevel() const;

	void BeginPlay();

	UFUNCTION(BlueprintCallable, Category = "Start")
	void StartGame() const;

protected:
	FTimerHandle CountdownTimerHandle;

    int32 CountdownTime = 5; // 倒计时从5开始

protected:
	void StartCountdown();

	void CountdownTick();
};



