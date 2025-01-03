// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Summary.generated.h"

/**
 * 
 */
class UVerticalBox;
UCLASS()

class TRY01_API USummary : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BluePrintReadOnly, meta = (bindWidget))
	UVerticalBox* ScoreContentBox;
	
public:
	UFUNCTION(BlueprintCallable)
	void AddPlayerScore(const FString& PlayerName, int32 PlayerScore);


	
	
};
