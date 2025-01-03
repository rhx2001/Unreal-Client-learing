// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TRY01_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdateGrade)
	uint32 PlayerScore;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsReady;
	
	bool bTakeGun;

public:
	UFUNCTION()
	void OnRep_UpdateGrade();

};
