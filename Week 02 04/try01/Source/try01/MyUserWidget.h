// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class TRY01_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BluePrintReadOnly, meta=(BindWidget))
	UTextBlock* ScoreTextBlock;


	
public:
	void UpdateUi(uint32 PlayerScore);


};
