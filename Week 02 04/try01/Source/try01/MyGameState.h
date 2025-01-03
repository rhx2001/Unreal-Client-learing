// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Summary.h"
#include "MyGameState.generated.h"


/**
 * 
 */
UCLASS()
class TRY01_API AMyGameState : public AGameState
{
	GENERATED_BODY()


	
public:
	AMyGameState();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CountDownTime)
	uint32 CountdownTime;

	UPROPERTY(EditAnywhere) 
	uint32 MaxCountTime = 5;


	FTimerHandle CountdownTimerHandle;

	UPROPERTY()
	USummary* SummaryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SummaryWidgetClass;



public:
	
	UFUNCTION()
	void OnRep_CountDownTime();
	
	UFUNCTION()
	void HandleCountTime();

	UFUNCTION(NetMulticast, Reliable)
	void ReportPlayerScore();

	void StartCountDown();

};
